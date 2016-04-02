/*
 * APA102C.c
 *
 * Created: 7/21/2015 14:51:49
 *  Author: Joshua
 */ 
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "APA102C.h"

#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define bit_val(byte,bit) (byte & (1 << bit)) // test for bit set

uint8_t portSet = 0;
void setPort(volatile uint8_t* port){
	portSet = 1;
	SPI_PORT = port;
}

//bit bangs an SPI signal to the specified pins of the given data
inline void sendBit(uint8_t clkPin, uint8_t datPin, uint8_t data){
	if(data){
		set(*SPI_PORT, datPin);
	}else{
		clear(*SPI_PORT, datPin);
	}
	set(*SPI_PORT, clkPin);
	clear(*SPI_PORT, clkPin);
}

//bit bangs an SPI signal to the specified pins of the given data
void sendByte(uint8_t clkPin, uint8_t datPin, uint8_t data){
	sendBit(clkPin, datPin, bit_val(data,7));
	sendBit(clkPin, datPin, bit_val(data,6));
	sendBit(clkPin, datPin, bit_val(data,5));
	sendBit(clkPin, datPin, bit_val(data,4));
	sendBit(clkPin, datPin, bit_val(data,3));
	sendBit(clkPin, datPin, bit_val(data,2));
	sendBit(clkPin, datPin, bit_val(data,1));
	sendBit(clkPin, datPin, bit_val(data,0));
}
//bit bangs an SPI signal to the specified pins that generates the specified color 
//	formatted for the APA102, provided as a byte array of R,G,B
void sendColor(uint8_t clkPin, uint8_t datPin,const uint8_t color[3]){
	if(!portSet){
		return;
	}
	//Start Frame
	sendByte(clkPin, datPin, 0x00);
	sendByte(clkPin, datPin, 0x00);
	sendByte(clkPin, datPin, 0x00);
	sendByte(clkPin, datPin, 0x00);
	//Data
	sendByte(clkPin, datPin, 0xE1);//Set brightness to current to minimum TODO: Add setBrightness function (0xE1...0xFF)
	sendByte(clkPin, datPin, color[2]);
	sendByte(clkPin, datPin, color[1]);
	sendByte(clkPin, datPin, color[0]);
}