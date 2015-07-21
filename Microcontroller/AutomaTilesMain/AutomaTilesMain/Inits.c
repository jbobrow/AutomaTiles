/*
 * Inits.c
 *
 * Created: 7/15/2015 11:55:59
 *  Author: Joshua
 */ 

#include "Inits.h"
#include "Pins.h"

void initIO(){//Set up pin directions, pull up/downs, overrides, pin change interrupts
	DDRA = IR; //Set IR LED pin as output
	PORTA = 0x00; //No pull ups and IR LED is low
	DDRB = LEDCLK|LEDDAT; //Set LED signals as outputs
	PORTB = BUTTON; //Apply pull up to button, reset is pulled up by default
	MCUCR|=(1<<ISC01)|(0<<ISC00);//Button interrupt is triggered on a falling edge
	PCMSK0 = 0x3F; //mask out only the phototransistors for the pin change interrupt
	GIMSK = (1<<INT0)|(0<<PCIE1)|(1<<PCIE0);//Enable INT0 interrupt for the button and Pin change interrupts for the phototransistors
	PRR = (1<<PRTIM1)|(1<<PRUSI);//Disable unused modules
}

void initAD(){//Set up all the necessary parameters for analog to digital conversion
	DIDR0 = MIC; //Disable digital buffering on microphone pin
	ADMUX = (1 << REFS1) | (0 << REFS0) // 1.1V ref
		| (0 << MUX5) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // (PA7 = MIC)
	ADCSRA = (1 << ADEN) // enable
		| (1 << ADATE) | (1 << ADIE) // auto trigger and interrupt enable
		| (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler /8
	ADCSRB = (0 << BIN) // single ended mode
		| (1 << ADLAR) // left align result to easily read only 8 MSB
		| (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0); // auto trigger off of A/D interrupt (free running mode)
	// start free running operation
	ADCSRA |= (1 << ADSC);
}

void initTimer(){//Set up global .1ms timer used for various protocols
	TCCR0A = (0<<COM0A1)|(0<<COM0A0)//OC0A Disconnected
		|(0<<COM0B1)|(0<<COM0B0)//OC0B Disconnected
		|(1<<WGM01)|(0<<WGM00);//Clear timer on compare match
	TCCR0B = (0<<WGM02)
		|(0<<CS02)|(0<<CS01)|(1<<CS00);//ClkIO/1 (1MHz)
	OCR0A = 100; //100 cycles at 1MHZ = .1ms
	TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);//Only enable OC Match A interrupt
}

