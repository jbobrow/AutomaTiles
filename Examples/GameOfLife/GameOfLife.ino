/*  Life-Like (based on Conway's Game of Life)
 *  
 *  Conway's Game of Life
 *  Each tile can be in a live or dead state based on its surrounding and its previous state
 *  In this variation, tiles are happy or sad according to their surroundings
 *  
 *  Red = sad
 *  Blue = happy
 *  
 *  happiness is contagious - if next to 2 or 3 blue tiles, become blue
 *  lonliness - if next to less than 2 blue tiles, become red
 *  overpopulation - if next to more than 3 blue tiles, become red
 *
 *  animation of a pulsing or heartbeat let's you know they are living
 *  flashing on the snap let's you know that they are listening
 *  
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  04.2016
 */

uint8_t neighbors[6];

uint8_t red[3] = {255,0,0};   // Red
uint8_t blue[3] = {0,0,255};  // Blue

// blink off color
uint8_t darkColor[3] = {0,0,0};

// the color we will actually show
uint8_t outputColor[3];

void setup() {
  setLongButtonCallback(button, 1000);  // setup a button handler (only for long press)
  setStepCallback(step);                // setup a step handler
  setState(0);                          // set initial state
  setMicOn();                           // listen to step forward
  setTimeout(600);                      // 10 minutes
}

void loop() {
  // animate color to pulse here
}

// this gets triggered when we press the button
void button() {
  setState((getState() + 1) % 2);
}

// this gets triggered when we snap
void step() {
  getNeighborStates(neighbors);

  uint8_t blueNeighbors = 0;
  for(uint8_t i=0; i<6; i++) {
    if(neighbors[i] == getState()) {
      blueNeighbors++;
    }
  }

  if(blueNeighbors < 2 || blueNeighbors > 3) {
    setState(0)
    setColor(red);
  } else {
    setState(1)
    setColor(blue);
  }
}
