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
 
void setup() {
  // Todo: handle button press and any other setup  
}

void draw() {
  // Todo: animate the colors from one state to the next through a smooth transition
}

void interpolateRGBColor(int[3] &result, int[3] from, int[3] to, float percent) {
  // first convert colors to HSV
  int[3] fromHSV = getHSVfromRGB(fromHSV, from);
  int[3] toHSV = getHSVfromRGB(toHSV, to);
  // tween between HSV values
  int[3] resultHSV;
  resultHSV[0] = fromHSV[0] * (1.0 - percent) + toHSV[0] * percent;
  resultHSV[1] = fromHSV[1] * (1.0 - percent) + toHSV[1] * percent;
  resultHSV[2] = fromHSV[2] * (1.0 - percent) + toHSV[2] * percent;
  getRGBfromHSV(result, resultHSV);
}

void getRGBfromHSV(int[3] &rgb, int[3] hsv) {

}

void getHSVfromRGB(int[3] &hsv, int[3] rgb) {

}

