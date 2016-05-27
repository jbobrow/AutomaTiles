/*   Hardware Test
 *   
 *   Summary: respond to button press, microphone, and each neighbor individually
 *
 *   IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *   into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *   Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *   Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *
 *   by Jonathan Bobrow
 *   05.2016
 */
 
uint8_t neighbors[6];
uint8_t displayColor[3];
uint8_t colors[7][3] = {{204,0,0},        // Red
                        {204,0,204},      // Magenta
                        {0,0,255},        // Blue
                        {0,204,204},      // Cyan
                        {0,204,0},        // Green
                        {204,204,0},      // Yellow
                        {204,64,0}};      // Orange

uint8_t brightness[60] = {
  64,71,77,84,90,96,102,107,
  112,116,119,122,125,127,128,128,
  128,127,125,122,119,116,112,107,
  102,96,90,84,77,71,64,57,
  51,44,38,32,26,21,16,12,
  9,6,3,1,0,0,0,1,
  3,6,9,12,16,21,26,32,
  38,44,51,57};
  
uint8_t darkColor[3] = {0,0,0};

uint32_t currentTime = 0;
uint32_t buttonPressTime = 0;
uint32_t snapTime = 0;

void setup() {
  setButtonCallback(button);  // setup a button handler
  setStepCallback(step);      // setup a step handler
  setState(0);                // set initial state
  setMicOn();                 // listen to step forward
  setTimeout(180);            // 3 minutes 
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = getTimer();

//  getNeighborStates(neighbors);

  if(currentTime - snapTime < 200) {
    displayColor[0] = 200;
    displayColor[1] = 200;
    displayColor[2] = 200;
    setColor(displayColor);  // flash white
  }
//  else if(neighbors[0] != 0){
//    // neighbor 0 is present
//  }
//  else if(neighbors[1] != 0){
//    // neighbor 1 is present    
//  }
//  else if(neighbors[2] != 0){
//    // neighbor 2 is present
//  }
//  else if(neighbors[3] != 0){
//    // neighbor 3 is present
//  }
//  else if(neighbors[4] != 0){
//    // neighbor 4 is present
//  }
//  else if(neighbors[5] != 0){
//    // neighbor 5 is present
//  }
  else {
    int8_t idx = (currentTime%3000)/50;
    displayColor[0] = 32 + brightness[idx]; 
    displayColor[1] = 0; 
    displayColor[2] = 32 + brightness[idx];
    setColor(displayColor); 
  }

}

void button() {
  
}

void step() {
  snapTime = getTimer();
}
