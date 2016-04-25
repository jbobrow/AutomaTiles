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

uint32_t waitTimeForStateChange = 5000;
uint32_t lastStateChange = 0;
uint8_t prevState = 1;

void setup() {
  // Todo: handle button press and any other setup 
  setButtonCallback(button);
  setMicOff();
  setTimeout(600);  // 10 minutes
  setState(1);
}

void loop() {
  // Todo: animate the colors from one state to the next through a smooth transition
  if(getTimer() - lastStateChange > waitTimeForStateChange) {
    getNeighborStates(neighbors);
    for(int i=0; i<6; i++) {
      if(neighbors[i] != getState()) {
          setState(neighbors[i]);
          lastStateChange = getTimer();
          break;
      }
    }
  }

  // if state is 1 then transition to be a solid color
  if(getState() == 1) {
    // send color to cyan
    toColor[0] = 0;
    toColor[1] = 255;
    toColor[2] = 255;
    if(prevState != 1) {
      // transition to color over the course of 1 second
      int diff = getTimer() - lastStateChange;
      float percent = constrain(diff / 1000, 0.0, 1.0);
      interpolateRGBColor(displayColor, fromColor, toColor, percent); 
      setColor(displayColor);
      if(percent >= 1.0) {
        prevState = 1;        
      }
    }
  }
  else if(getState() == 2) {
    // if state is 2 then animate through the spectrum
    // send color to magenta to test static color
//    fromColor[0] = 255;
//    fromColor[1] = 0;
//    fromColor[2] = 255;
//    setColor(fromColor);

    // animate through colors
    int diff = getTimer() - lastStateChange;
    float percent = (diff % 5000) / 5000.0; // through all of the colors in 5 seconds
    getRGBSpectrumFromPercent(displayColor, percent); 
    setColor(displayColor);
    fromColor[0] = displayColor[0];
    fromColor[1] = displayColor[1];
    fromColor[2] = displayColor[2];
  }
}

// change state from 1 to 2 or from 2 to 1
void button() {
  setState((getState() % 2) + 1);
}

void getRGBSpectrumFromPercent(uint8_t *rgbColor, float percent) {
  hsv hsvColor;
  hsvColor.h = percent * 360;
  hsvColor.s = 1.0;
  hsvColor.v = 1.0;
  rgb convertedColor;
  convertedColor = hsv2rgb(hsvColor);
  rgbColor[0] = int(convertedColor.r * 255);
  rgbColor[1] = int(convertedColor.g * 255);
  rgbColor[2] = int(convertedColor.b * 255);
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

void getRGBfromHSV(uint8_t *rgbColor, hsv hsvColor) {
  rgb convertedColor;
  convertedColor = hsv2rgb(hsvColor);
  rgbColor[0] = int(convertedColor.r * 255);
  rgbColor[1] = int(convertedColor.g * 255);
  rgbColor[2] = int(convertedColor.b * 255);
}

hsv getHSVfromRGB(uint8_t rgbColor[3]) {
  rgb toConvert;
  toConvert.r = rgbColor[0] / 255.0;
  toConvert.g = rgbColor[1] / 255.0;
  toConvert.b = rgbColor[2] / 255.0;
  return rgb2hsv(toConvert);
}

