/*
 * AutomaTiles.h
 *
 * Created: 1/6/2016 11:56:26
 *  Author: Joshua
 */ 


#ifndef AUTOMATILES_H_
#define AUTOMATILES_H_

#include <stdint.h>

typedef void (*cb_func)(void);

void tileSetup(void);
void getNeighborStates(uint8_t * result);
void sendStep(void);
uint32_t getTimer(void);
void setColor(const uint8_t color[3]);
void setState(uint8_t state);
uint8_t getState(void);
void setStepCallback(cb_func cb);
void setButtonCallback(cb_func cb);
void setLongButtonCallback(cb_func cb);
void setLongButtonCallback(cb_func cb, uint16_t ms);
void setLongButtonCallbackTime(uint16_t ms);
void setTimerCallback(cb_func cb, uint16_t t);
void setTimerCallbackTime(uint16_t t);
void setTimeout(uint16_t seconds);
void setMicOn();
void setMicOff();

#define PULSE_WIDTH 4
extern volatile int16_t holdoff;
extern uint8_t outColor[3];
extern volatile uint8_t wake;

volatile static uint8_t progDir = 0;//direction to pay attention to during programming. Set to whichever side put the module into program mode.
volatile static uint8_t comBuf[65];//buffer for holding communicated messages when programming rules (oversized)
volatile static uint16_t bitsRcvd = 0;//tracking number of bits received for retransmission/avoiding overflow

#endif /* AUTOMATILES_H_ */