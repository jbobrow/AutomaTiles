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

#include "Pins.h"
#include "Inits.h"
#include "APA102C.h"


volatile static int16_t holdoff = 10;//for temporarily preventing click outputs
volatile static uint8_t click = 0;//becomes non-zero when a click is detected
volatile static uint8_t state = 0;//current state of tile
volatile static uint32_t timer = 0;//.1 ms timer tick
volatile static uint32_t times[6][4];
volatile static uint8_t timeBuf[6];

const uint8_t colors[][3] = 
{
	{0x00,0x00,0x00},
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

const uint8_t dark[] = {0x00,0x00,0x00};
const uint8_t light[] = {0x55,0x55,0x55};
	
static void getStates(uint8_t * result);

int main(void)
{
	//Initialization routines
	initIO();
	setPort(&PORTB);
	sendColor(LEDCLK,LEDDAT,dark);
	sei();
	initAD();
	initTimer();
	for(uint8_t i = 0; i<6; i++){
		timeBuf[i]=0;
	}
	
    while(1)
    {
		/*if(click){
			uint8_t states[6];
			getStates(states);
			uint8_t numOn = 0;
			for (uint8_t i = 0; i< 6; i++)
			{
				if(states[i]>0){
					numOn++;
				}	
			}
			
			if(numOn != 1){
				state = !state;
			}
			holdoff = 500;
			click = 0;
		}*/
		
		if(!(timer & 0x3F)){
			sendColor(LEDCLK, LEDDAT, colors[state]);
		}
	}
}

static void getStates(uint8_t * result){
	cli();
	uint32_t curTime = timer;
	uint32_t diffs[3];
	sei();
	for(uint8_t i = 0; i < 6; i++){
		if((curTime-times[i][timeBuf[i]])>100){//More than .1 sec since last pulse, too long
			result[i] = 0;
		}else{//received pulses recently
			uint8_t buf = timeBuf[i];
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
				if(diffs[0] == diffs[1] || diffs[0] == diffs[2]){
					result[i] = (uint8_t) diffs[0];
				}else if(diffs[1] == diffs[2]){//too much variation
					result[i] = (uint8_t) diffs[1];
				}else{
					result[i] = 0;
				}
			}
		}
	}
}

ISR(TIM0_COMPA_vect){
	timer++;
	uint8_t cycle = (uint8_t) timer%(24*state+4);
	if(cycle==5){
		PORTB |= IR;//Set pin high
		DDRB |= IR;//Set direction out
	}else if(cycle==6){
		DDRB &= ~IR;//Set direction in
		PORTB &= ~IR;//Set pin tristated
	}else{
		DDRB &= ~IR;//Set direction in
		PORTB &= ~IR;//Set pin tristated
		if(PINB & BUTTON){//Button active high
			if(holdoff==0){
				state = !state;//simple setup for 2 state tile
			}
			holdoff = 500;//debounce and hold state until released
		}
	}
}

ISR(INT0_vect){//INT0 interrupt triggered when the pushbutton is pressed
	/*if(holdoff==0){
		state = !state;//simple setup for 2 state tile
		holdoff = 500;
	}*/
}

ISR(PCINT0_vect){//Pin Change 0 interrupt triggered when any of the phototransistors change level
	static uint8_t prevVals = 0;
	uint8_t vals = PINA & 0x3f;
	uint8_t newOn = vals & ~prevVals;
	for(uint8_t i = 0; i < 6; i++){
		if(newOn & 1<<i){
			timeBuf[i]++;
			timeBuf[i] &= 0x03;
			times[i][timeBuf[i]] = timer;
		}
	}
	
	prevVals = vals;
}



ISR(ADC_vect){
	//Values saved for derivative calculation
	static uint16_t median = 1<<10;
	static uint16_t medDelta = 1<<5;
	
	uint8_t adc;	
	
	adc = ADCH;// Record ADC value
	sei(); //re-enable interrupts, allow this calculation to be interrupted
	
	//update median
	if((adc<<8)<median){// update running median. Error on high side. note that due to comparison, the median is scaled up by 8
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
	if((delta<<5)<medDelta && medDelta > 1){// update running median. Error on high side. note that due to comparison, the median is scaled up by 8
		medDelta--;
		}else{
		medDelta++;
	}
	if(holdoff<0){
		holdoff = 0;
	}
	if(holdoff == 0){//holdoff can be set in main to disable click being set for a period of time
		if(medDelta < delta){//check for click. as the median is scaled up by 16, an exceptional event is needed.
			click = delta;
		}
	}else{
		holdoff--;
	}
}