/*   Wire World (Simulation/Visualization)
 *   
 *   Summary: each tile simulates traces in an wiring diagram. Electrons travel
 *            through conductive tiles with a head an a tail. Logic gates can be
 *            constructed through clever geometry. 
 *            Read more here: https://en.wikipedia.org/wiki/Wireworld
 *
 *   Rules: Based on Brian Silverman's Wire World
 *
 *   Electron Head: becomes electron tail
 *         Display blue 
 *   Electron Tail: becomes conductor
 *         Display red
 *   Conductor: Electron head if exactly one or two of the neighbouring cells are electron heads, or remains Conductor otherwise.
 *         Display yellow
 *
 *   IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *   into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *   Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *   Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *
 *   by Jonathan Bobrow
 *   03.2016
 */
 
uint8_t neighbors[6];
uint8_t colors[4][3] = {{0,0,0},        // NONE
						{0,32,255},     // Electron Head
                        {255,0,0},      // Electron Tail
                        {180,120,8}};   // Conductor


void setup() {
  setButtonCallback(button);  // setup a button handler
  setStepCallback(step);      // setup a step handler
  setState(1);                // set initial state
  setMicOn();                 // listen to step forward
}

void loop() {
  // put your main code here, to run repeatedly:
}

void button() {
  // rotate through 3 states (HEAD, TAIL, CONDUCTOR)
  setState((getState()%3)+1);
  setColor(colors[getState()]);
}

void step() {
  // check neighbors
  getNeighborStates(neighbors);
  
  switch(getState()) {
    case 1: 
    	 setState(2);	// become an electron tail
         break;
    case 2: 
    	 setState(3);	// become a conductor
         break;
    case 3: 
    	 uin8_t count = 0;
         for(uint8_t i=0; i<6; i++) {
         	if(neighbors[i] == 1)
         		count++;
         }
         if(count == 1 || count == 2)
         	setState(1);	// become electron head
         else
         	set State(3);	// stay a conductor
         break;
    default: break;
  }
  setColor(colors[getState()]);
}
