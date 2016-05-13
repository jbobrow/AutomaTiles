/*  Color Gossip (Visualization)
 *  
 *  Pressing on an AutomaTile assigns it a new color and it infects its neighbors
 *  with the color as well. Colors should animate through the HSV spectrum to arrive
 *  at their new color.
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
#include "color.h"

// colors arranged to travel the farthest in transition between colors
uint8_t colors[7][3] = {{204,0,0},        // Red
                        {0,204,204},      // Cyan
                        {204,0,204},      // Magenta
                        {204,204,0},      // Yellow
                        {0,0,255},        // Blue
                        {204,64,0},       // Orange
                        {0,204,0}};       // Green

uint8_t loadingColor[3] = {128,128,128};
uint8_t completeColor[3] = {255,255,255};
uint8_t darkColor[3] = {0,0,0};
uint8_t displayColor[3];

uint8_t neighbors[6];

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds

uint32_t waitTimeForStateChange = 5000;
uint32_t timeSinceLastStateChange = 0;
uint8_t curState = 1;
uint8_t prevState = 1;

void setup() {
  setButtonCallback(button);
  setState(curState);
  setColor(colors[curState-1]);
  setMicOff();
  setTimeout(600);  // 10 minute
}

void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > updateFrequency) {
    // Todo: animate the colors from one state to the next through a smooth transition
    getNeighborStates(neighbors);
    uint8_t didChangeState = 0;
    uint8_t isAlone = 1;
    for(int i=0; i<6; i++) {
      if(neighbors[i] != 0) {
        isAlone = 0;
        if(curTime - timeSinceLastStateChange > waitTimeForStateChange) {
          if(!didChangeState) { // no need to keep checking if we will change state
            if(neighbors[i] != curState ) {
              curState = neighbors[i];
              setState(curState);
              timeSinceLastStateChange = curTime;
              didChangeState = 1;
            }
          }
        }
      }
    }
    
    if(isAlone) {// isAlone) {
      // blink orange
      if((curTime/1000) % 2 == 0) {
        setColor(colors[5]);
      }
      else {
        setColor(darkColor);
      }
    }
    else {
      // if need to change, change, otherwise, display color of state
      if( prevState != curState) {
        // transition to color over the course of 2 seconds
        uint32_t diff = curTime - timeSinceLastStateChange;
        float percent = diff/2000.0;
        interpolateRGBColor(displayColor, colors[prevState-1], colors[curState-1], percent); 
        setColor(loadingColor);
        if(percent >= 1.0) {
          prevState = curState;
          setColor(completeColor);
        }
      }
      else {
        setColor(colors[curState - 1]);
      }
    }
    
    // update our update time
    lastUpdateTime = curTime;
  }
}

// change state and color
void button() {
  curState = (curState % 7) + 1;
  setState(curState);
  timeSinceLastStateChange = getTimer();
}

void interpolateRGBColor(uint8_t *result, uint8_t from[3], uint8_t to[3], float percent) {
  // first convert colors to HSV
  hsv fromHSV = getHSVfromRGB(from);
  hsv toHSV = getHSVfromRGB(to);
  // tween between HSV values
  hsv resultHSV;
  resultHSV.h = fromHSV.h * (1.0 - percent) + toHSV.h * percent;
  resultHSV.s = fromHSV.s * (1.0 - percent) + toHSV.s * percent;
  resultHSV.v = fromHSV.v * (1.0 - percent) + toHSV.v * percent;
  getRGBfromHSV(result, resultHSV);
}

void getRGBfromHSV(uint8_t *result, hsv hsvColor) {
  rgb convertedColor;
  convertedColor = hsv2rgb(hsvColor);
  result[0] = int(convertedColor.r * 255.0);
  result[1] = int(convertedColor.g * 255.0);
  result[2] = int(convertedColor.b * 255.0);
}

hsv getHSVfromRGB(uint8_t rgbColor[3]) {
  rgb toConvert;
  toConvert.r = rgbColor[0] / 255.0;
  toConvert.g = rgbColor[1] / 255.0;
  toConvert.b = rgbColor[2] / 255.0;
  return rgb2hsv(toConvert);
}

