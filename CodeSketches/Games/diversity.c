#include "AutomaTiles.h"

uint8_t diversityValue;
uint8_t possibleDiversityValue;

uint8_t neighbors[6];

uint8_t colors[4][3] = {{255,0,0},         // Red
                         {0,255,0},        // Green
                         {0,0,255},        // Blue
                         {255,0,255},        // Magenta
                         {255,255,0},        // Yellow
                         {0,255,255}};        // Cyan

uint8_t outputColor[3];

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

  // set our initial state
  setState(1);

  while(1){
    // loop over and over
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
   	uint8_t happiness = (diversityValue * 6) / (possibleDiversityValue); 
   	if(possibleDiversityValue < 2)
   		happiness = 0;

   	// sadness is the inverse of happiness
   	uint8_t sadness = 6 - happiness;

   	// now decide how happy we are
   	outputColor[0] = colors[getState()-1][0] >> sadness;	// Red output value
   	outputColor[1] = colors[getState()-1][1] >> sadness;	// Green output value
   	outputColor[2] = colors[getState()-1][2] >> sadness;	// Blue output value

   	setColor(outputColor);
  }
}