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
void setTimerCallback(cb_func cb, uint16_t t);
void setTimerCallbackTime(uint16_t t);
void setTimeout(uint16_t seconds);
void setMicOn();
void setMicOff();

#define PULSE_WIDTH 8
extern volatile int16_t holdoff;
extern uint8_t outColor[3];
extern volatile uint8_t wake;

extern volatile uint8_t progDir;
extern volatile uint8_t comBuf[65];
extern volatile uint8_t datBuf[64];
extern volatile uint16_t bitsRcvd;
extern volatile uint32_t modeStart;

#endif /* AUTOMATILES_H_ */