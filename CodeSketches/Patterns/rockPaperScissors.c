// Basic sketch to test 3 state AutomaTiles
// rock, paper, scissors
// turn into the domininant type if surrounded

#include "AutomaTiles.h"

uint8_t neighbors[6];
uint8_t colors[3][3] = {{255,255,0},
                         {255,0,255},
                         {0,255,255}};

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
          if(neighbors[i] == opponent)
               counter++;
     }

     if(counter >= 2) {
          // set to opponent type
          setState(opponent);
          setColor(colors[opponent - 1]);
     }
     // otherwise remain same
}

int main(void) {
     // setup the automatile
     tileSetup();

     // setup a button handler
     setButtonCallback(button);

     // setup a step handler
     setStepCallback(step);

     // set initial state
     setState(1);

     // Keep the processor running “
     while(1){
          // handle any continuous logic here
     }
}
