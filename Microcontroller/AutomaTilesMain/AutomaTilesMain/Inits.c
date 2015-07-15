/*
 * Inits.c
 *
 * Created: 7/15/2015 11:55:59
 *  Author: Joshua
 */ 

#include "Inits.h"
#include "Pins.h"

void initIO(){//Set up pin directions, pull up/downs, overrides
	DDRA = IR; //Set IR LED pin as output
	PORTA = 0x00; //No pull ups and IR LED is low
	DDRB = LEDCLK|LEDDAT; //Set LED signals as outputs
	PORTB = BUTTON; //Apply pull up to button, reset is pulled up by default
}

void initAD(){//Set up all the necessary parameters for analog to digital conversion
	DIDR0 = MIC; //Disable digital buffering on microphone pin
	ADMUX = (1 << REFS1) | (0 << REFS0) // 1.1V ref
		| (0 << MUX5) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // (PA7 = MIC)
	ADCSRA = (1 << ADEN) // enable
		| (1 << ADATE) | (1 << ADIE) // auto trigger and interrupt enable
		| (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler /64
	ADCSRB = (0 << BIN) // single ended mode
		| (1 << ADLAR) // left align result to easily read only 8 MSB
		| (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0); // auto trigger off of A/D interrupt (free running mode)
}

void initTimer(){//Set up global .1ms timer used for various protocols
	
}