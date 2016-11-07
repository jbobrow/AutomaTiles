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

uint8_t darkColor[3] = {0,0,0};
uint8_t brightness[30] = {
  71,84,96,107,
  116,122,127,128,
  127,122,116,107,
  96,84,71,57,
  44,32,21,12,
  6,1,0,1,
  6,12,21,32,
  44,57};

uint8_t displayColor[3];

uint8_t neighbors[6];

uint32_t lastUpdateTime = 0;
uint16_t updateFrequency = 50;  //milliseconds

uint16_t waitTimeForStateChange = 5000;
uint32_t timeSinceLastStateChange = 0;
uint8_t curState = 1;
uint8_t prevState = 1;

uint8_t aloneCount = 0;

void setup() {
  setButtonCallback(button);
  setState(curState);
  setColor(colors[curState-1]);
  setMicOff();
  setTimeout(300);  // 5 minute
}

void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > updateFrequency) {
    // Todo: animate the colors from one state to the next through a smooth transition
    getNeighborStates(neighbors);
    uint8_t didChangeState = 0;
    if(aloneCount < 254)
      aloneCount++;
    for(uint8_t i=0; i<6; i++) {
      if(neighbors[i] != 0) {
        aloneCount = 0;
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
    
    if(aloneCount > 10) {// isAlone) {
      //if just pressed
      if(curTime - timeSinceLastStateChange < 500) {
        setColor(colors[curState - 1]);
      }
      else {
        //pulse white
        int8_t idx = (curTime%3000)/100;
        displayColor[0] = 32 + brightness[idx]; 
        displayColor[1] = 32 + brightness[idx]; 
        displayColor[2] = 32 + brightness[idx];
        setColor(displayColor); 
      }
    }
    else {
      // if need to change, change, otherwise, display color of state
      if( prevState != curState) {
        // transition to color over the course of 2 seconds
        uint32_t diff = curTime - timeSinceLastStateChange;
        float percent = diff/2000.0;
        interpolateRGBColor(displayColor, colors[prevState-1], colors[curState-1], percent); 
        setColor(displayColor);
        if(percent >= 1.0) {
          prevState = curState;
          setColor(colors[curState-1]);
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
  //Determine quickest route to color
  if(abs(fromHSV.h - toHSV.h) <= 180.0) {
    // straight shot, just lerp to the color
    resultHSV.h = fromHSV.h * (1.0 - percent) + toHSV.h * percent;
  }
  else {
    // quickest way is to go around, like a clock
    double first, second, total;
    if(fromHSV.h > toHSV.h) {
      // hue path illustration: |>>second>>*-------------*>>first>>|
      first = 360.0 - fromHSV.h;
      second = toHSV.h;
      total = first + second;

      if(percent < first/total) {
        resultHSV.h = fromHSV.h + (percent * total);
      }
      else {
        resultHSV.h = toHSV.h + ((percent - 1.0) * total);
      }
    }
    else {
      // hue path illustration: |<<first<<*-------------*<<second<<|
      first = fromHSV.h;
      second = 360.0 - toHSV.h;
      total = first + second;

      if(percent < first/total) {
        resultHSV.h = fromHSV.h - (percent * total);
      }
      else {
        resultHSV.h = toHSV.h + ((percent - 1.0) * total);
      }
    }
  }
  
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

