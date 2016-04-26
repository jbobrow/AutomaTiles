/*  Prototyper
 *  
 *  1. Press on a tile to change it from steady to blink
 *  2. Long press (1 second) on a tile to change it to the next color
 *  3. Long press (3 seconds) to gossip the color to all neighbors 
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

uint8_t colors[7][3] = {{204,0,0},        // Red
                        {204,0,204},      // Magenta
                        {0,0,255},        // Blue
                        {0,204,204},      // Cyan
                        {0,204,0},        // Green
                        {204,204,0},      // Yellow
                        {204,64,0}};      // Orange

// blink off color
uint8_t darkColor[3] = {0,0,0};
uint8_t isBlinking = 0;
uint8_t currentColorIndex = 0;

uint8_t neighbors[6];

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds
uint32_t lastPressTime = 0;

void setup() {
  setButtonCallback(button);
  setLongButtonCallback(longPress, 1000);
  setState(0);
  setColor(colors[0]);
  setMicOff();
  setTimeout(600);  // 10 minute
}

void loop() {
  if(getTimer() - lastUpdateTime > updateFrequency) {
//    getNeighborStates(neighbors);
//    for(uint8_t i=0; i<6; i++){
//      // check neighbors for something
//    }

    // handle blinking
    if(isBlinking) {
      if((getTimer() / (500 / isBlinking)) % 2) {
        setColor(darkColor);
      }
      else {
        setColor(colors[currentColorIndex]);
      }
    }
    else {
      setColor(colors[currentColorIndex]);
    }
    
    lastUpdateTime = getTimer();
  }
}

void button() {
  if(getTimer() - lastPressTime < 1000) {
    // double press
    if(currentColorIndex == 0) {
      currentColorIndex = 6;
    }
    else {
      currentColorIndex = (currentColorIndex-1) % 7;
    }
    setColor(colors[currentColorIndex]);
    isBlinking = 0;    
  }
  else {
    // single press
    isBlinking = (isBlinking+1) % 3;
  }
  lastPressTime = getTimer();
}

void longPress() {
  currentColorIndex = (currentColorIndex+1) % 7;
  setColor(colors[currentColorIndex]);
  isBlinking = 0;
}
