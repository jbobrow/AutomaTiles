/*
 * AutomaTilesMain.c
 *
 * Created: 7/14/2015 15:53:13
 *  Author: Joshua
 */ 


#include <inttypes.h>

#include "AutomaTiles.h"

// init to the default rulestate for automatiles
volatile static uint8_t birthRules[7] = {0,0,1,1,0,0,0}; // if true, should be born
volatile static uint8_t deathRules[7] = {1,1,0,0,1,1,1}; // if true, should die (gotta find a better metaphor, this is too sad)
volatile static uint8_t numStates = 2; // sets the age of an automatile

uint8_t colors[][3] = //index corresponds to state
{
	{0x55,0x00,0x00},
	{0x00,0x55,0xAA},
	{0x7F,0x7F,0x00},
	{0xAA,0x55,0x00},
	{0xFF,0x00,0x00},
	{0xAA,0x00,0x55},
	{0x7F,0x00,0x7F},
	{0x55,0x00,0xAA},
	{0x00,0x00,0xFF},
	{0x00,0x55,0xAA},
	{0x00,0x7F,0x7F},
	{0x00,0xAA,0x55},		
	{0x00,0xFF,0x00},
	{0x55,0xAA,0x00}
};

uint8_t bright(uint8_t val){
	if(val >= 127){
		return 255;
	}
	if(val == 0){
		return 4;
	}
	return val<<1;
}

uint8_t red[] = {0xFF, 0x00, 0x00};
void button(){
	setColor(red);
}

uint8_t blue[] = {0x00, 0x00, 0xFF};
void click(){
	setColor(blue);
}

int main(void)
{
	tileSetup();
	setButtonCB(button);
	setClickCB(click);
		
    while(1)
    {	
		/*if(mode==running){
			if(click){
				uint8_t neighborStates[6];
				getStates(neighborStates);
				uint8_t clickColor[] = {bright(colors[state][0]),bright(colors[state][1]),bright(colors[state][2])}; 
				sendColor(LEDCLK, LEDDAT, clickColor);
				uint8_t numOn = 0;

				sync = 3;//request sync pulse be sent at next possible opportunity (set to 4 for logistical reasons)

				for (uint8_t i = 0; i< 6; i++)
				{
					//at the moments specific state detection is a bit iffy, 
					//so mapping any received state>1 to a state of 1 works for now
					if(neighborStates[i]>0){
						numOn++;
					}	
				}
				//Logic for what to do to the state based on number of on neighbors
				if(state == 0) {
					if(birthRules[numOn]) {
						state=1;
					}
				}
				else {
					if(deathRules[numOn]) {
						state = (state + 1) % numStates;
					}
				}
				//End logic
			}
		}*/
	}
}