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

#endif /* AUTOMATILES_H_ */