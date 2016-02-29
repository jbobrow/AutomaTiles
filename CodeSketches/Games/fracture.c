/*	Fracture (Game)
 *	
 *	Setup: For 3-6 players. Each player gets 5-6 tiles of a specific color.
 *	       Game starts will all tiles connected, but completely segregated by color.
 *
 *	Goal: Get your tiles to be touching only other players tiles.
 *	      Note: your tiles must be touching two other tiles to feel happily diverse.
 *
 *	Turns: Start from a player of choice and then go clockwise taking turns.
 *	       A turn consists of fracturing the population into two sub populations,
 *	       and then connecting them back together in any way you please.
 *	
 *	When a player has a tile fully diversified, it will blink,
 *	first player to have all of their tiles blink wins. 
 *
 *  Game devopment by: Celia Pearce, Jeanie Choi, Isabella Carlson, Mike Lazer-Walker, Joshua Sloane
 *
 *	by Jonathan Bobrow
 *	01.28.2016
 */

#include "AutomaTiles.h"

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;	//milliseconds

uint8_t diversityValue;
uint8_t possibleDiversityValue;

uint8_t neighbors[6];

uint8_t colors[6][3] = {{255,0,0},          // Red
                         {0,255,0},         // Green
                         {0,0,255},         // Blue
                         {255,0,255},       // Magenta
                         {255,255,0},       // Yellow
                         {0,255,255}};      // Cyan

// blink off color
uint8_t darkColor[3] = {0,0,0};

// the color we will actually show
uint8_t outputColor[3];

// minimum number of neighbors present
// to be happy at all (i.e. tiles with only 
// one neighbor, feel left out)
uint8_t minNeighbors = 2; 

// this gets triggered when we press the button
void button() {
	// update the color/type we are
	// increment our state value by one between the range of (1-6)
	setState(getState()%6 + 1);
}

void step() {
	// Do nothing here
}

int main(void) {      
   tileSetup(); 
   setButtonCallback(button); 
   setStepCallback(step); 
   
   // turn off the mic, we aren't using it
   setMicOff();

   // set our initial state
   setState(1);

   // set the sleep timer
   setTimeout(3600);	// 1 hour

	// begin loop over and over
	while(1){
		if(getTimer() - lastUpdateTime > updateFrequency) {

		 	getNeighborStates(neighbors);

		 	// get diversity
		 	diversityValue = 0;
		 	possibleDiversityValue = 0;
		 	for(uint8_t i=0; i<6; i++) {
		 		// check if we have a neighbor
		 		if(neighbors[i] != 0) {
		 			possibleDiversityValue++;
		 			// now we have a neighbor check its state
			 		if(neighbors[i] != getState()) {
		 				diversityValue++;
		 			}
		 		}
		 	}

		 	// happiness integer (0-6)
		 	uint8_t happiness = possibleDiversityValue == 0 ? 6 : (diversityValue * 6) / (possibleDiversityValue); 
		 	if(possibleDiversityValue < minNeighbors)
		 		happiness = 0;

		 	// sadness is the inverse of happiness
		 	uint8_t sadness = 6 - happiness;

		 	// now make the light bright based on how happy we are
		 	// (or dim it based on how sad we are...)
		 	outputColor[0] = colors[getState()-1][0] >> sadness;	// Red output value
		 	outputColor[1] = colors[getState()-1][1] >> sadness;	// Green output value
		 	outputColor[2] = colors[getState()-1][2] >> sadness;	// Blue output value
			/*  NOTE: BITSHIFT OPERATOR
		 	 *  >>n operator simply divides the value by 2^n
		 	 *  (i.e. 128 >> 2 = 32 (128/2^2) -> (128/4) -> 32)
		 	 *  super handy for quick division or multiplication by powers of 2
		 	 */
		 	
			// to help players know when the brightest value is showing
		 	// blink if in happiest state or simply display the output color
		    if(happiness == 6 &&(getTimer() % 500) > 250 ) {
					setColor(darkColor);
			}else {
					setColor(outputColor);	
			}

		 	// update our update time
		 	lastUpdateTime = getTimer();
		}
	}
}
