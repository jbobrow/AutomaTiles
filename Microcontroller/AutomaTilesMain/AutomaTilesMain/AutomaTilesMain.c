#include "AutomaTiles.h"

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;	//milliseconds

uint8_t diversityValue;
uint8_t possibleDiversityValue;

uint8_t neighbors[6];

uint8_t colors[6][3] = {{255,0,0},         // Red
{0,255,0},        // Green
{0,0,255},        // Blue
{255,0,255},        // Magenta
{255,255,0},        // Yellow
{0,255,255}};        // Cyan

uint8_t darkColor[3] = {0,0,0};	// blink off

uint8_t outputColor[3];

uint8_t minNeighbors = 2;

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
	setTimeout(3);	// 1 hour
	
	uint8_t happiness = 0;

	while(1){
		// loop over and over
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
			happiness = possibleDiversityValue == 0 ? 6 : (diversityValue * 6) / (possibleDiversityValue);
			if(possibleDiversityValue < minNeighbors)
			happiness = 0;

			// sadness is the inverse of happiness
			uint8_t sadness = 6 - happiness;

			// now decide how happy we are
			outputColor[0] = colors[getState()-1][0] >> sadness;	// Red output value
			outputColor[1] = colors[getState()-1][1] >> sadness;	// Green output value
			outputColor[2] = colors[getState()-1][2] >> sadness;	// Blue output value

			// update our update time
			lastUpdateTime = getTimer();
		}
		
		// blink if in happiest state
		if(happiness == 6 && (getTimer() % 500) > 250 ) {
			setColor(darkColor);
			}else {
			setColor(outputColor);
		}	
	}
}