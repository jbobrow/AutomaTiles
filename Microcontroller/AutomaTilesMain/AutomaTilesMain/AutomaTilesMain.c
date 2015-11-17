/*
 * AutomaTilesMain.c
 *
 * Created: 7/14/2015 15:53:13
 *  Author: Joshua
 */ 


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <avr/sleep.h>

#include "Pins.h"
#include "Inits.h"
#include "APA102C.h"

volatile static int16_t holdoff = 2000;//for temporarily preventing click outputs
volatile static uint8_t click = 0;//becomes non-zero when a click is detected
volatile static uint8_t sync = 0;//becomes non-zero when synchronization pulses need to be sent out
volatile static uint8_t state = 0;//current state of tile
volatile static uint32_t timer = 0;//.1 ms timer tick
volatile static uint32_t times[6][4];//ring buffer for holding leading  detection edge times for the phototransistors
volatile static uint8_t timeBuf[6];//ring buffer indices

const uint32_t TIMEOUT = 10;
volatile static uint32_t sleepTimer = 0;
volatile static uint32_t powerDownTimer = 0;
volatile static uint8_t wake = 0;

const uint8_t PULSE_WIDTH = 8;//1/2 bit of manchester encoding, time in ms
volatile static uint8_t progDir = 0;//direction to pay attention to during programming. Set to whichever side put the module into program mode.
volatile static uint8_t comBuf[64];//buffer for holding communicated messages when programming rules (oversized)
volatile static uint16_t bitsRcvd = 0;//tracking number of bits received for retransmission/avoiding overflow
static uint8_t seqNum = 0;//Sequence number used to prevent circular retransmission of data

// init to the default rulestate for automatiles
volatile static uint8_t birthRules[7] = {0,0,1,1,0,0,0}; // if true, should be born
volatile static uint8_t deathRules[7] = {1,1,0,0,1,1,1}; // if true, should die (gotta find a better metaphor, this is too sad)
volatile static uint8_t numStates = 2; // sets the age of an automatile
volatile static uint8_t soundEn = 1; //if true, react to sound

uint8_t colors[][3] = //index corresponds to state
{
	{0x55,0x00,0x00},
	{0x55,0x55,0x55},
	{0x7F,0x7F,0x00},
	{0xAA,0x55,0x00},
	{0xFF,0x00,0x00},
	{0xAA,0x00,0x55},
	{0x7F,0x00,0x7F},
	{0x55,0x00,0xAA},
	{0x00,0x00,0xFF},
	{0x00,0x55,0xAA},
	{0x00,0x7F,0x7F},
	{0x00,0xAA,0x55},		
	{0x00,0xFF,0x00},
	{0x55,0xAA,0x00}
};

const uint8_t dark[3] = {0x00, 0x00, 0x00};
const uint8_t bright[3] = {0xFF, 0xFF, 0xFF};
const uint8_t recieveColor[3] = {0x00, 0x7F, 0x00};
const uint8_t transmitColor[3] = {0xAA, 0x55, 0x00};

enum MODE
{
	sleep,
	running,
	recieving,
	transmitting
} mode;
	
static void getStates(uint8_t * result);
static void parseBuffer();

int main(void)
{
	//Initialization routines
	initIO();
	setPort(&PORTB);
	sendColor(LEDCLK,LEDDAT,colors[0]);
	sei();
	initAD();
	initTimer();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	//Set up timing ring buffers
	for(uint8_t i = 0; i<6; i++){
		timeBuf[i]=0;
	}
	
	mode = running;
	
    while(1)

    {	
		if(mode==sleep){
			disAD();
			//
			DDRB &= ~IR;//Set direction in
			PORTB &= ~IR;//Set pin tristated
			sendColor(LEDCLK, LEDDAT, dark);
			PORTA |= POWER;//Set LED and Mic power pin high (off)
			wake = 0;
			while(!wake){
				if(timer-powerDownTimer>5000){
					sleep_cpu();
					wake = 1;
				}
			}
			holdoff=500;
			PORTA &= ~POWER;
			enAD();
			powerDownTimer = timer;
			sleepTimer = timer;
			mode = running;
		}else if(mode==running){
			if(click){
				uint8_t neighborStates[6];
				getStates(neighborStates);
				sendColor(LEDCLK, LEDDAT, bright);
				uint8_t numOn = 0;

				sync = 3;//request sync pulse be sent at next possible opportunity (set to 4 for logistical reasons)

				for (uint8_t i = 0; i< 6; i++)
				{
					//at the moments specific state detection is a bit iffy, 
					//so mapping any received state>1 to a state of 1 works for now
					if(neighborStates[i]>0){
						numOn++;
					}	
				}
				//Logic for what to do to the state based on number of on neighbors
				if(state == 0) {
					if(birthRules[numOn]) {
						state=1;
					}
				}
				else {
					if(deathRules[numOn]) {
						state = (state + 1) % numStates;
					}
				}
				//End logic

				//prevent another click from being detected for a bit
				holdoff = 500;
				click = 0;
			}
		
			//periodically update LED once every 0x3F = 64 ms (fast enough to feel responsive)
			if(!(timer & 0x3F)){
				sendColor(LEDCLK, LEDDAT, colors[state]);
			}
			
			//check if we enter sleep mode
			if(timer-sleepTimer>1000*TIMEOUT){
				mode = sleep;
			}
		}else if(mode==recieving){
			//disable A/D
			disAD();
			//set photo transistor interrupt to only trigger on specific direction
			setDir(progDir);
			//set recieving color
			sendColor(LEDCLK, LEDDAT, recieveColor);	
			//record time entering the mode for timeout
			uint32_t modeStart = timer;
			while(mode==recieving){//stay in this mode until instructed to leave or timeout
				if(timer-modeStart>3000){//been in mode 1 for more than 5 seconds
					mode = transmitting;					
				}
			}
		}else if(mode==transmitting){
			//disable Phototransistor Interrupt
			setDirNone();
			//set LED to output
			DDRB |= IR;//Set direction out
			//send 5 pulses
			uint32_t startTime = timer;
			if(bitsRcvd>=8 && comBuf[0]!=seqNum){
				for(int i=0; i<5; i++){
					while(timer==startTime){
						PORTB &= ~IR;
					}
					startTime = timer;
					while(timer==startTime){
						PORTB |= IR;
					}
					startTime = timer;
				}
				parseBuffer();
			}else{
				bitsRcvd = 0;
			}
			
			startTime = timer;
			sendColor(LEDCLK, LEDDAT, transmitColor);//update color while waiting			
			while(timer<startTime+20);//pause for mode change
			startTime = timer;
			uint16_t timeDiff;
			uint16_t bitNum;
			while(bitsRcvd>0){
				timeDiff = (timer-startTime)/PULSE_WIDTH;
				bitNum = timeDiff/2;
				if(timeDiff%2==0){//first half
					if(comBuf[bitNum/8]&(1<<bitNum%8)){//bit high
						PORTB &=  ~IR;
					}else{//bit low
						PORTB |=  IR;
					}
				}else{//second half
					if(comBuf[bitNum/8]&(1<<bitNum%8)){//bit high
						PORTB |=  IR;
						}else{//bit low
						PORTB &=  ~IR;
					}
				}
				if(bitNum>=bitsRcvd){
					bitsRcvd = 0;
				}				
			}
			//while(timer<startTime+2000);//pause for effect
			
			//done transmitting
			//re-enable A/D
			enAD();
			//re-enable all phototransistors
			setDirAll();
			state = 0;
			mode = running;

		}
	}
}

/* Receives two bytes containing birth rules and death rules
 * Each byte contains 0 or 1 in each position to 
 * represent true or false if born or killed off with n neighbors
 */
static void setRules(uint8_t br, uint8_t dr) {
	birthRules[0] = br&(1<<0);
	birthRules[1] = br&(1<<1);
	birthRules[2] = br&(1<<2);
	birthRules[3] = br&(1<<3);
	birthRules[4] = br&(1<<4);
	birthRules[5] = br&(1<<5);
	birthRules[6] = br&(1<<6);
	
	deathRules[0] = dr&(1<<0);
	deathRules[1] = dr&(1<<1);
	deathRules[2] = dr&(1<<2);
	deathRules[3] = dr&(1<<3);
	deathRules[4] = dr&(1<<4);
	deathRules[5] = dr&(1<<5);
	deathRules[6] = dr&(1<<6);
}

/* Set the number of states each automatile will go through
 * this is effectively an age... i.e. after this many die states, 
 * it will actually die 
 */
static void setNumStates(uint8_t num) {
	numStates = num;
}

static void setColors(volatile uint8_t* buf){
	colors[0][0] = buf[0];
	colors[0][1] = buf[1];
	colors[0][2] = buf[2];
	colors[1][0] = buf[3];
	colors[1][1] = buf[4];
	colors[1][2] = buf[5];
}

/* Uses the current state of the times ring buffer to determine the states of neighboring tiles
 * For each side, to have a non-zero state, a pulse must have been received in the last 100 ms and two of the
 * last three timing spaces must be equal.
 * 
 * State is communicated as a period for the pulses. Differences are calculated between pulses and if a consistent
 * difference is found, that translates directly to a state
 * Accuracy is traded for number of states (i.e. 5 states can be communicated reliably, while 10 with less robustness)
*/
static void getStates(uint8_t * result){
	cli();//Disable interrupts to safely grab consistent timer value
	uint32_t curTime = timer;
	uint32_t diffs[3];
	for(uint8_t i = 0; i < 6; i++){
		if((curTime-times[i][timeBuf[i]])>100){//More than .1 sec since last pulse, too long
			result[i] = 0;
		}else{//received pulses recently
			uint8_t buf = timeBuf[i];//All bit-masking is to ensure the numbers are between 0 and 3
			diffs[0] = times[i][buf] - times[i][(buf-1)&0x03];
			diffs[1] = times[i][(buf-1)&0x03] - times[i][(buf-2)&0x03];
			diffs[2] = times[i][(buf-2)&0x03] - times[i][(buf-3)&0x03];
			if(diffs[0]>100 || diffs[1]>100 || diffs[2] > 100){//Not enough pulses recently
				result[i] = 0;
			}else{//received enough pulses recently
				//rounding 
				diffs[0] >>= 3;
				diffs[1] >>= 3;
				diffs[2] >>= 3;
				//checking if any two of the differences are equal and using a value from the equal pair
				if(diffs[0] == diffs[1] || diffs[0] == diffs[2]){
					result[i] = (uint8_t) diffs[0];
				}else if(diffs[1] == diffs[2]){
					result[i] = (uint8_t) diffs[1];
				}else{//too much variation
					result[i] = 0;
				}
			}
		}
	}	
	sei();//Re-enable interrupts
}

//parse received data buffer and update values
//1 -> next 2 bytes are rules
//2 -> next byte is number of states
//3 -> next 6 bytes are colors
//4 -> disable mic
//5 -> enable mic
static void parseBuffer(){
	seqNum = comBuf[0];
	int i = 1;
	uint8_t numBytes = bitsRcvd/8;
	while(i<numBytes){
		if(comBuf[i]==1){
			if(i+2<numBytes){
				setRules(comBuf[i+1],comBuf[i+2]); 
			}
			i+=3;
		}else if(comBuf[i]==2){
			if(i+1<numBytes){
				setNumStates(comBuf[i+1]);
			}
			i+=2;
		}else if(comBuf[i]==3){
			if(i+6<numBytes){
				setColors(comBuf+i+1);
			}
			i+=7;
		}else if(comBuf[i]==4){
			soundEn = 0;
			i++;
		}else if(comBuf[i]==5){
			soundEn = 1;
			i++;
		}else{
			i++;
		}
	}
}

//Timer interrupt occurs every 1 ms
//Increments timer and controls IR LEDs to keep their timing consistent
ISR(TIM0_COMPA_vect){
	static uint8_t IRcount = 0;//Tracks cycles for accurate IR LED timing
	static uint8_t sendState = 0;//State currently being sent. only updates on pulse to ensure accurate states are sent
	timer++;
	if(mode==running){
		IRcount++;
		if(IRcount>=(uint8_t)(sendState*8+4)){//State timings are off by 4 from a multiple of 8 to help with detection
			IRcount = 0;
			if(sync==0){
				sendState = state;
			}
		}
		
		if(IRcount==5){ 
			PORTB |= IR;
			DDRB |= IR;
		}else if(IRcount==7&&sync>1){
			PORTB |= IR;
			DDRB |= IR;		
			sync = 1;
		}else if(IRcount==9&&sync==1){
			PORTB |= IR;
			DDRB |= IR;
			sync = 0;
		}else if(sendState==0&&sync>0){//0 case is special
			if((IRcount&0x01)!=0){
				PORTB |= IR;
				DDRB |= IR;			
				sync -= 1;
				}else{
				DDRB &= ~IR;//Set direction in
				PORTB &= ~IR;
			}
		}else{
			DDRB &= ~IR;//Set direction in
			PORTB &= ~IR;//Set pin tristated
		
			if(IRcount<5){
				if(PINB & BUTTON){//Button active high
					if(holdoff==0){
						state = !state;//simple setup for 2 state tile
						sleepTimer = timer;
						powerDownTimer = timer;
					}
					holdoff = 500;//debounce and hold state until released
				}
			}
		}
	}else if(mode == recieving){//recieving data, ensure led off
		DDRB &= ~IR;//Set direction in
		PORTB &= ~IR;//Set pin tristated		
	}
}


//INT0 interrupt triggered when the pushbutton is pressed
ISR(PCINT1_vect){
	wake = 1;
}

//Pin Change 0 interrupt triggered when any of the phototransistors change level
//Checks what pins are newly on and updates their buffers with the current time
ISR(PCINT0_vect){
	static uint8_t prevVals = 0; //stores the previous state so that only what pins are newly on are checked
	static uint8_t pulseCount[6]; //stores counted pulses for various actions
	static uint32_t oldTime = 0; //stored the previous time for data transmission
	uint8_t vals = PINA & 0x3f; //mask out phototransistors
	uint8_t newOn = vals & ~prevVals; //mask out previously on pins
	
	powerDownTimer = timer;
	
	if(mode==running){
		for(uint8_t i = 0; i < 6; i++){
			if(newOn & 1<<i){ //if an element is newly on, 
				if(timer-times[i][timeBuf[i]]<10){//This is a rapid pulse. treat like a click
					pulseCount[i]++;
					if(pulseCount[i]==2){
						if(holdoff==0){
							click = 1;
							wake = 1;
							sleepTimer = timer;
						}
					}
					if(pulseCount[i]>=4){//There have been 4 quick pulses. Enter programming mode.							
						mode = recieving;
						progDir = i;
					}					
				}else{//Normally timed pulse, process normally
					pulseCount[i]=0;
					timeBuf[i]++;
					timeBuf[i] &= 0x03;
					times[i][timeBuf[i]] = timer;
				}
			}
		}
	}else if(mode==recieving){
		if(((prevVals^vals)&(1<<progDir))){//programming pin has changed
			if(timer-oldTime > (3*PULSE_WIDTH)/2){//an edge we care about
				if(timer-oldTime > 4*PULSE_WIDTH){//first bit. use for sync
					bitsRcvd = 0;
					for(int i = 0; i < 64; i++){//zero out buffer
						comBuf[i]=0;
					}					
				}
				oldTime = timer;				
				if(bitsRcvd<64*8){
					uint8_t bit = ((vals&(1<<progDir))>>progDir);
					comBuf[bitsRcvd/8] |= bit<<(bitsRcvd%8);
					bitsRcvd++;
				}				
			}
		}
	}
	prevVals = vals;
}

//ADC conversion complete interrupt
//Calculates a running median for zeroing out signal
//Then calculates a running median of deltas from the median to check for exceptional events
//If a delta is very high compared to the median, a click is detected and click is set to non-0
ISR(ADC_vect){
	//Values saved for derivative calculation
	static uint16_t median = 1<<15;
	static uint16_t medDelta = 1<<5;
	
	uint8_t adc;	
	
	adc = ADCH;// Record ADC value
	sei(); //re-enable interrupts, allow this calculation to be interrupted
	
	//update running median. Error on high side.
	//note that due to comparison, the median is scaled up by 2^8
	if((adc<<8)<median){
		median--;
		}else{
		median++;
	}
	uint16_t delta;
	if(median > (adc<<8)){// Calculate delta
		delta = (median>>8)-adc;
		}else{
		delta = adc-(median>>8);
	}
	
	//Update running delta median. Error on high side.
	//note that due to comparison, the median is scaled up by 2^4=16
	if((delta<<4)<medDelta && medDelta > 10){ 
		medDelta--;
		}else{
		medDelta++;
	}
	
	if(holdoff<0){//edge case protection
		holdoff = 0;
	}
	
	if(holdoff == 0){//holdoff can be set elsewhere to disable click being set for a period of time
		if(medDelta < delta){//check for click. as the median delta is scaled up by 16, an exceptional event is needed.
			if(soundEn){
				click = delta;//Board triggered click as soon as it could (double steps)
				sleepTimer = timer;
			}
		}
	}else{
		holdoff--;
	}
}