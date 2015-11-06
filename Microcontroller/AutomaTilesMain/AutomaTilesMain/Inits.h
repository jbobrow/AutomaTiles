/*
 * Inits.h
 *
 * Created: 7/15/2015 11:56:20
 *  Author: Joshua
 */ 

#ifndef INITS_H_
#define INITS_H_
#include "Pins.h"

void initIO(void);
void initAD(void);
void disAD(void);
void enAD(void);
void initTimer(void);
void setDir(uint8_t dir);
void setDirNone(void);
void setDirAll(void);
#endif /* INITS_H_ */