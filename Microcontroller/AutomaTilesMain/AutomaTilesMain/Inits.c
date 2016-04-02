/*
 * Inits.c
 *
 * Created: 7/15/2015 11:55:59
 *  Author: Joshua
 */ 

#include "Inits.h"

void initIO(){//Set up pin directions, pull up/downs, overrides, pin change interrupts
	DDRA = POWER; //Set IR LED pin as output
	PORTA = 0x00; //No pull ups and POWER is set low
	DDRB = LEDCLK|LEDDAT; //Set LED signals as outputs
	PORTB = 0x00; //No pull ups and IR LED is low
	PCMSK1 = BUTTON; //Mask out only the button for the pin change interrupt
	PCMSK0 = 0x3F; //Mask out only the phototransistors for the pin change interrupt
	GIMSK = (1<<PCIE1)|(1<<PCIE0);//Enable Pin change interrupt for the button and Pin change interrupts for the phototransistors
	PRR = (1<<PRTIM1)|(1<<PRUSI);//Disable unused modules
}

void initAD(){//Set up all the necessary parameters for analog to digital conversion
	DIDR0 = MIC; //Disable digital buffering on microphone pin
	ADMUX = (0 << REFS1) | (0 << REFS0) // VCC ref
		| (0 << MUX5) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // (PA7 = MIC)
	ADCSRA = (1 << ADEN) // enable
		| (1 << ADATE) | (1 << ADIE) // auto trigger and interrupt enable
		| (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0); // prescaler /16
	ADCSRB = (0 << BIN) // single ended mode
		| (1 << ADLAR) // left align result to easily read only 8 MSB
		| (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0); // auto trigger off of A/D interrupt (free running mode)
	// start free running operation
	ADCSRA |= (1 << ADSC);
}

void disAD(){//disable microphone when not in use
	ADCSRA = (0 << ADEN) // disable
	| (1 << ADATE) | (1 << ADIE) // auto trigger and interrupt enable
	| (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0); // prescaler /16
}

void enAD(){//re-enable microphone
	ADCSRA = (1 << ADEN) // ensable
	| (1 << ADATE) | (1 << ADIE) // auto trigger and interrupt enable
	| (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0); // prescaler /16
	// start free running operation
	ADCSRA |= (1 << ADSC);
}

void setDir(uint8_t dir){//make tile only listen to one phototransistor
	PCMSK0 = 1<<dir;
}

void setDirNone(){
	PCMSK0 = 0;
}

void setDirAll(){//make tile listen to all phototransistors
	PCMSK0 = 0x3F;
}

void initTimer(){//Set up global .1ms timer used for various protocols
	TCCR0A = (0<<COM0A1)|(0<<COM0A0)//OC0A Disconnected
		|(0<<COM0B1)|(0<<COM0B0)//OC0B Disconnected
		|(1<<WGM01)|(0<<WGM00);//Clear timer on compare match
	TCCR0B = (0<<WGM02)
		|(0<<CS02)|(1<<CS01)|(0<<CS00);//ClkIO/8 (125kHz)
	OCR0A = 125; //125 cycles at 125kHZ = 1ms
	TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);//Only enable OC Match A interrupt
}

