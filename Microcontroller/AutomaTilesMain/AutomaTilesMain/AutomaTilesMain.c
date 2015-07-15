/*
 * AutomaTilesMain.c
 *
 * Created: 7/14/2015 15:53:13
 *  Author: Joshua
 */ 


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Pins.h"
#include "Inits.h"

volatile uint16_t holdoff = 10;//for temporarily preventing click outputs
volatile uint8_t click = 0;//becomes non-zero when a click is detected
int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

ISR(INT0_vect){//INT0 interrupt triggered when the pushbutton is pressed
	
}

ISR(PCINT0_vect){//Pin Change 0 interrupt triggered when any of the phototransistors change level
	
}

//Values saved for derivative calculation
volatile uint8_t adcOld = 0;
volatile uint8_t adcNew = 0;
volatile uint16_t medDelta = 1;
ISR(ADC_vect){
	adcNew = ADCH;// Record ADC value
	sei(); //re-enable interrupts, allow this calculation to be interrupted
	
	int delta;
	if(adcNew > adcOld){// Calculate delta
		delta = adcNew-adcOld;
		}else{
		delta = adcOld-adcNew;
	}
	
	adcOld = adcNew;// update old value
	if(holdoff == 0){//holdoff can be set in main to disable click being set for a period of time
		if(medDelta < delta){//check for click as the median is scaled up by 16, and exceptional event is needed.
			click = delta;
		}
		}else{
		holdoff--;
	}
	if((delta<<4)<medDelta){// update running median. Error on high side. note that due to comparison, the median is scaled up by 16
		medDelta--;
		}else{
		medDelta++;
	}
}