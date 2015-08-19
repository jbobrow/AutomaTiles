/*
 * APA102C.h
 *
 * Custom Library for writing values to the APA102C RGB LED
 *
 * Created: 7/21/2015 15:11:37
 *  Author: Joshua
 */ 


#ifndef APA102C_H_
#define APA102C_H_

void setPort(volatile uint8_t* port);
void sendColor(uint8_t clkPin, uint8_t datPin,const uint8_t color[3]);
volatile uint8_t* SPI_PORT;

#endif /* APA102C_H_ */