/*  Color Spin (Visualization)
 *  
 *  Simply cycles color from orange to magenta over 6 seconds
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

// colors for cycle
uint8_t startColor[3] = {204,64,0}; // orange
uint8_t endColor[3] = {204,0,204};  // magenta
uint8_t displayColor[3];

uint8_t neighbors[6];

uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds

uint32_t waitTimeForStateChange = 5000;
uint32_t timeSinceLastStateChange = 0;
uint8_t curState = 1;
uint8_t prevState = 1;

void setup() {
  setState(0);
  setColor(startColor);
  setMicOff();
  setTimeout(600);  // 10 minute
}

void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > updateFrequency) {

    uint32_t diff = curTime%6001;
    float percent = constrain(diff/6000.0, 0.0, 1.0);
    interpolateRGBColor(displayColor, startColor, endColor, percent); 
    setColor(displayColor);
    
    // update our update time
    lastUpdateTime = curTime;
  }
}

// change state and color
void button() {
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

