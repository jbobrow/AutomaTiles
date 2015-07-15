/*
 * Pins.h
 *
 * Created: 7/15/2015 10:59:51
 *  Author: Joshua
 */ 
#include <avr/io.h>

#ifndef PINS_H_
#define PINS_H_
//Phototransistors are mapped out of order for routing purposes
#define PHOTO0 1<<PA5
#define PHOTO1 1<<PA4
#define PHOTO2 1<<PA3
#define PHOTO3 1<<PA0
#define PHOTO4 1<<PA1
#define PHOTO5 1<<PA2
//IR LED output pin
#define IR 1<<PA6
//LED clock and data pins
#define LEDCLK 1<<PB0
#define LEDDAT 1<<PB1
//Microphone pin
#define MIC 1<<PA7
//Pushbutton pin
#define BUTTON 1<<PB2
#endif /* PINS_H_ */