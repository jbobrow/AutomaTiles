#include <AutomaTiles.h>
#include <Arduino.h>
#include "APA102C.h"

uint32_t timer;
const uint8_t black[3] = {0x00, 0x00, 0x00};
int main(void) {
	tileSetup();
	
	setup();
	
	timer = getTimer();
	while(1){
		uint32_t diff = getTimer()-timer;
		if(holdoff<diff){
			holdoff = 0;
		}else{
			holdoff -= diff;
		}
		uint8_t t = timer%64;
		if(t<=32 && t+diff>=32){
			sendColor(LEDCLK, LEDDAT,outColor);
		}
		timer = getTimer();
		
		if(timeout>0){
			if(timer-sleepTimer>1000*timeout){
				mode = sleep;
				disAD();
				DDRB &= ~IR;//Set direction in
				PORTB &= ~IR;//Set pin tristated
				sendColor(LEDCLK, LEDDAT, black);
				PORTA |= POWER;//Set LED and Mic power pin high (off)
				wake = 0;
			}
		}
		
		loop();
	}
}