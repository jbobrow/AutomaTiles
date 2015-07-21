/*
 * APA102C.h
 *
 * Created: 7/21/2015 15:11:37
 *  Author: Joshua
 */ 


#ifndef APA102C_H_
#define APA102C_H_

void setPort(volatile uint8_t* port);
void sendColor(uint8_t clkPin, uint8_t datPin,uint8_t color[3]);
volatile uint8_t* SPI_PORT;

#endif /* APA102C_H_ */