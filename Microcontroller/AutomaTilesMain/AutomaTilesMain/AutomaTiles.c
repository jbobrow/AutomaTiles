/*
 * AutomaTiles.c
 *
 * Created: 1/6/2016 11:56:47
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
#include "AutomaTiles.h"

volatile static int16_t holdoff = 2000;//for temporarily preventing click outputs
volatile static uint8_t click = 0;//becomes non-zero when a click is detected
volatile static uint8_t sync = 0;//becomes non-zero when synchronization pulses need to be sent out
volatile static uint8_t state = 0;//current state of tile
volatile static uint32_t timer = 0;//.1 ms timer tick
volatile static uint32_t times[6][4];//ring buffer for holding leading  detection edge times for the phototransistors
volatile static uint8_t timeBuf[6];//ring buffer indices
volatile static uint8_t soundEn = 1; //if true, react to sound

const uint32_t TIMEOUT = 20;
volatile static uint32_t startTime = 0;
volatile static uint32_t sleepTimer = 0;
volatile static uint32_t powerDownTimer = 0;
volatile static uint8_t wake = 0;

const uint8_t PULSE_WIDTH = 8;//1/2 bit of manchester encoding, time in ms
volatile static uint8_t progDir = 0;//direction to pay attention to during programming. Set to whichever side put the module into program mode.
volatile static uint8_t comBuf[64];//buffer for holding communicated messages when programming rules (oversized)
volatile static uint16_t bitsRcvd = 0;//tracking number of bits received for retransmission/avoiding overflow
//static uint8_t seqNum = 0;//Sequence number used to prevent circular retransmission of data

const uint8_t dark[3] = {0x00, 0x00, 0x00};
const uint8_t recieveColor[3] = {0x00, 0x7F, 0x00};
const uint8_t transmitColor[3] = {0xAA, 0x55, 0x00};
static uint8_t outColor[3] = {0x00, 0x00, 0xFF};

enum MODE
{
	sleep,
	running,
	recieving,
	transmitting
} mode;

/* Uses the current state of the times ring buffer to determine the states of neighboring tiles
 * For each side, to have a non-zero state, a pulse must have been received in the last 100 ms and two of the
 * last three timing spaces must be equal.
 * 
 * State is communicated as a period for the pulses. Differences are calculated between pulses and if a consistent
 * difference is found, that translates directly to a state
 * Accuracy is traded for number of states (i.e. 5 states can be communicated reliably, while 10 with less robustness)
*/
void getStates(uint8_t * result){
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

void tileSetup(void){
	//Initialization routines
	initIO();
	setPort(&PORTB);
	sendColor(LEDCLK,LEDDAT,dark);
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
}

void emptyCB(void){
	return;
}

cb_func clickCB = emptyCB;
cb_func buttonCB = emptyCB;
cb_func pulseCB = emptyCB;

void setColor(const uint8_t color[3]){
	outColor[0] = color[0];
	outColor[1] = color[1];
	outColor[2] = color[2];
}

void setClickCB(cb_func cb){
	clickCB = cb;
}

void setButtonCB(cb_func cb){
	buttonCB = cb;
}
//onClick
//onPress
//data[]


//Timer interrupt occurs every 1 ms
//Increments timer and controls IR LEDs to keep their timing consistent
ISR(TIM0_COMPA_vect){
	static uint8_t IRcount = 0;//Tracks cycles for accurate IR LED timing
	static uint8_t sendState = 0;//State currently being sent. only updates on pulse to ensure accurate states are sent
	timer++;
	
	if(timer-sleepTimer>1000*TIMEOUT){
		mode = sleep;
		disAD();
		DDRB &= ~IR;//Set direction in
		PORTB &= ~IR;//Set pin tristated
		sendColor(LEDCLK, LEDDAT, dark);
		PORTA |= POWER;//Set LED and Mic power pin high (off)
		wake = 0;
	}
	
	if(mode==running){
		//periodically update LED once every 0x3F = 64 ms (fast enough to feel responsive)
		if(!(timer & 0x3F)){
			sendColor(LEDCLK, LEDDAT, outColor);
		}
		
		//check if a click has happened and call appropriate function
		if(click){
			clickCB();
			holdoff = 100;
			click = 0;
		}
		
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
						buttonCB();
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
	}else if(mode==sleep){
		uint32_t diff = timer-powerDownTimer;
		if(diff>500){
			sei();
			sleep_cpu();
			cli();
			wake = 1;
		}
		if(wake == 1){
			holdoff=500;
			startTime = timer;
			PORTA &= ~POWER;
			wake = 2;
		}else if (wake == 2)
		{
			if(timer-startTime>250){
				wake=3;
			}
		}else if (wake == 3)
		{
			DDRB |= IR;//Set direction out
			PORTB |= IR;//Set pin on
			sendColor(LEDCLK, LEDDAT, transmitColor);
			startTime = timer;
			wake = 4;
		}else if(wake == 4){
			if(timer-startTime>500){
				wake=5;
			}
		}else if(wake == 5){
			enAD();
			powerDownTimer = timer;
			sleepTimer = timer;
			mode = running;
		}
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
				cli();
				sleepTimer = timer;
				sei();
			}
		}
		}else{
		holdoff--;
	}
}