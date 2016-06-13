/*  Fracture (Game)
 *  
 *  Setup: For 3-6 players. Each player gets 5-6 tiles of a specific color.
 *         Game starts will all tiles connected, but completely segregated by color.
 *
 *  Goal: Get your tiles to be touching only other players tiles.
 *        Note: your tiles must be touching two other tiles to feel happily diverse.
 *
 *  Turns: Start from a player of choice and then go clockwise taking turns.
 *         A turn consists of fracturing the population into two sub populations,
 *         and then connecting them back together in any way you please.
 *  
 *  When a player has a tile fully diversified, it will blink,
 *  first player to have all of their tiles blink wins. 
 *
 *  Game devopment by: Celia Pearce, Jeanie Choi, Isabella Carlson, Mike Lazer-Walker, Joshua Sloane
 *
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  01.28.2016
 */

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds

uint8_t diversityValue;
uint8_t possibleDiversityValue;

uint32_t lastTimeColorChanged = 0;

uint8_t neighbors[6];
uint8_t prevNeighbors[6] = {255,255,255,255,255,255};

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

// brightness
uint8_t brightness[7] = {128,149,170,191,212,233,255};

// minimum number of neighbors present
// to be happy at all (i.e. tiles with only 
// one neighbor, feel left out)
uint8_t minNeighbors = 2; 

// only update the state if the neighbors are confirmed to change
uint8_t didNeighborsChangeCount = 0;
uint8_t didNeighborsChangeCountThreshold = 10;

// blinking for happiness
uint8_t isBlinking = 0;

// this gets triggered when we press the button
void button() {
  // update the color/type we are
  // increment our state value by one between the range of (1-6)
  if(getTimer() - lastTimeColorChanged > 2000) {
    setState(getState()%6 + 1);
    didNeighborsChangeCount = 10; // make sure to update color and state
    prevNeighbors[0] = 255; // make sure at least one neighbor is different
    isBlinking = 0;
  }
  lastTimeColorChanged = getTimer();
}

void step() {
  // Do nothing here
}

void setup() {
  setLongButtonCallback(button, 2000);  // setup a button handler (only for long press)
  setStepCallback(step);          // setup a step handler
  setState(1);                    // set initial state
  setMicOff();                    // listen to step forward
  setTimeout(600);                // 10 minutes
}

void loop() {
   
  if(getTimer() - lastUpdateTime > updateFrequency) {

    getNeighborStates(neighbors);

    // check if we have a different neighbor
    for(uint8_t i=0; i<6; i++) {
      if(neighbors[i] != prevNeighbors[i]) {
        didNeighborsChangeCount++;
        break;
      }

      if(i==5) {
        didNeighborsChangeCount = 0;
      }
    }

    // if the surroundings changed
    if(didNeighborsChangeCount >= 10) {

      // reset count
      didNeighborsChangeCount = 0;
      
      // update prev neighbors
      for(uint8_t i=0; i<6; i++) {
        prevNeighbors[i] = neighbors[i];
      }
      
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
    
      // now make the light bright based on how happy we are
      outputColor[0] = int(colors[getState()-1][0] / 255.0 * brightness[happiness]);  // Red output value
      outputColor[1] = int(colors[getState()-1][1] / 255.0 * brightness[happiness]);  // Green output value
      outputColor[2] = int(colors[getState()-1][2] / 255.0 * brightness[happiness]);  // Blue output value
      
      // to help players know when the brightest value is showing
      // blink if in happiest state or simply display the output color
      if(happiness == 6) {
        isBlinking = 1;
      }
      else {
        isBlinking = 0;
      }    
    }

    if(isBlinking && (getTimer() % 500) > 250) {
        setColor(darkColor);
    }else {
        setColor(outputColor);  
    }

    
    // update our update time
    lastUpdateTime = getTimer();
  }
}
