/*  Rock, Paper, Scissors (experiment)
 *  
 *  A basic test for multistate AutomaTiles
 *  
 *  In this example, there are 3 colors and states
 *  
 *  Yellow is stronger than Cyan, but weaker than Magenta
 *  Magenta is stronger than Yellow, but weaker than Cyan
 *  Cyan is stronger than Magenta, but weaker than Yellow
 *  Y < M < C < Y...
 *  
 *  The weaker color will conform to the stronger color 
 *  if 2 or more neighboring tiles are of the stronger color
 *  
 *  Reference for this kind of reaction
 *  http://ncase.me/emoji-prototype/?remote=-K71iWhftjtOIfx6b2Fo
 *  
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  --------------------------------------------------------------------------------------------------
 *   
 *  by Jonathan Bobrow + Joshua Sloane
 *  02.2016
 */

uint8_t neighbors[6];
uint8_t colors[3][3] = {{255,255,0},   // Yellow 
                         {255,0,255},  // Magenta
                         {0,255,255}}; // Cyan

void setup() {
  setButtonCallback(button);
  setStepCallback(step);
  setState(1);
}

void loop() {
  // all updates happen from either a button press or a step in time
}

void button() {
  // change tile to next state
  setState(getState()%3 + 1);
  setColor(colors[getState()-1]);
}

void step() {
  // check neighbors
  getNeighborStates(neighbors);
  
  // determine opponent
  uint8_t opponent = getState()%3 + 1;
  
  // if two or more of greater value (dominant opponent)
  uint8_t counter = 0;
  for(uint8_t i=0; i<6; i++) {
    if(neighbors[i] == opponent) {
      counter++;
    }
  }
  
  if(counter >= 2) {
    // set to opponent type
    setState(opponent);
    setColor(colors[opponent - 1]);
  }
  // otherwise remain same
}
