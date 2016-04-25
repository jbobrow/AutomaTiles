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

// default colors to yellow
uint8_t fromColor[3] = {255,255,0};
uint8_t toColor[3] = {255,255,0};
uint8_t displayColor[3];

uint8_t neighbors[6];

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds

uint32_t waitTimeForStateChange = 5000;
uint32_t lastStateChange = 0;
uint8_t prevState = 0;
uint8_t isAlone = 1;

void setup() {
  // Todo: handle button press and any other setup 
  setButtonCallback(button);
  setMicOff();
  setTimeout(60);  // 1 minute
  setState(2);
  setColor(fromColor);
}

void loop() {
  if(getTimer() - lastUpdateTime > updateFrequency) {
    // Todo: animate the colors from one state to the next through a smooth transition
    getNeighborStates(neighbors);
    uint8_t counter = 0;
    for(int i=0; i<6; i++) {
      if(neighbors[i] != 0) {
        if(neighbors[i] != getState() ) {
          if(getTimer() - lastStateChange > waitTimeForStateChange) {
            setState(neighbors[i]);
            lastStateChange = getTimer();
            //break;
          }
        }
      }
      else {
        counter++;
      }
    }
    if(counter == 6) {
      isAlone = 1;
    }
    else {
      isAlone = 0;
    }
    
    if(isAlone) {
      // send color red
      toColor[0] = 255;
      toColor[1] = 0;
      toColor[2] = 0;
      if(prevState != 1) {
        // transition to color over the course of 1 second
        uint32_t diff = getTimer() - lastStateChange;
        float percent = constrain(diff / 2000.0, 0.0, 1.0);
        interpolateRGBColor(displayColor, fromColor, toColor, percent); 
        setColor(displayColor);
        if(percent >= 1.0) {
          prevState = 1;
          fromColor[0] = 255;
          fromColor[1] = 0;
          fromColor[2] = 0;                
        }
      }
    }
    else if(getState() == 2) {
    // if state is 1 then transition to be a solid color
      // send color to cyan
      toColor[0] = 0;
      toColor[1] = 255;
      toColor[2] = 255;
      if(prevState != 2) {
        // transition to color over the course of 1 second
        uint32_t diff = getTimer() - lastStateChange;
        float percent = constrain(diff / 2000.0, 0.0, 1.0);
        interpolateRGBColor(displayColor, fromColor, toColor, percent); 
        setColor(displayColor);
        if(percent >= 1.0) {
          prevState = 2;
          fromColor[0] = 0;
          fromColor[1] = 255;
          fromColor[2] = 255;        
        }
      }
    }
    else if(getState() == 3) {
      // if state is 2 then animate through the spectrum
      prevState = 3;
      // animate through colors
      uint32_t diff = getTimer() - lastStateChange;
      float percent = (diff % 5000) / 5000.0; // through all of the colors in 5 seconds
      getRGBSpectrumFromPercent(displayColor, percent); 
      setColor(displayColor);
      fromColor[0] = displayColor[0];
      fromColor[1] = displayColor[1];
      fromColor[2] = displayColor[2];
    }
    // update our update time
    lastUpdateTime = getTimer();
  }
}

// change state from 1 to 2 or from 2 to 1
void button() {
  if(getState() == 2)
    setState(3);
  else
    setState(2);
  lastStateChange = getTimer();
}

void getRGBSpectrumFromPercent(uint8_t *rgbColor, float percent) {
  hsv hsvColor;
  hsvColor.h = percent * 360;
  hsvColor.s = 1.0;
  hsvColor.v = 1.0;
  getRGBfromHSV(rgbColor, hsvColor);
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
  result[0] = int(convertedColor.r * 255);
  result[1] = int(convertedColor.g * 255);
  result[2] = int(convertedColor.b * 255);
}

hsv getHSVfromRGB(uint8_t rgbColor[3]) {
  rgb toConvert;
  toConvert.r = rgbColor[0] / 255.0;
  toConvert.g = rgbColor[1] / 255.0;
  toConvert.b = rgbColor[2] / 255.0;
  return rgb2hsv(toConvert);
}

