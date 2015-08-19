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

volatile static uint16_t holdoff = 10;//for temporarily preventing click outputs
volatile static uint8_t click = 0;//becomes non-zero when a click is detected
volatile static uint8_t state = 0;//current state of tile
register uint16_t timer asm("r2");//.1 ms timer tick
//volatile static uint16_t 

const uint8_t colors[][3] = 
{
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
const uint8_t light[] = {0x55, 0x55, 0x55};

int main(void)
{
	//Initialization routines
	setPort(&PORTB);
	initIO();
	sei();
	initAD();
	sendColor(LEDCLK,LEDDAT,dark);
	
    while(1)
    {
		if(click){
			
		}
	}
}

ISR(TIM0_COMPA_vect){
	timer++;
}

ISR(INT0_vect){//INT0 interrupt triggered when the pushbutton is pressed
	state = !state;
}

ISR(PCINT0_vect){//Pin Change 0 interrupt triggered when any of the phototransistors change level
	static uint8_t prevVals = 0;
	uint8_t vals = PINA & 0x3f;
	uint8_t newOn = vals & ~prevVals;
	for(uint8_t i = 0; i < 6; i++){
		if(newOn & 1<<i){
			
		}
	}
	prevVals = vals;
}



ISR(ADC_vect){
	//Values saved for derivative calculation
	static uint16_t median = 1<<12;
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
	if(holdoff == 0){//holdoff can be set in main to disable click being set for a period of time
		if(medDelta < delta){//check for click. as the median is scaled up by 16, an exceptional event is needed.
			click = delta;
		}
	}else{
		holdoff--;
	}
	
}