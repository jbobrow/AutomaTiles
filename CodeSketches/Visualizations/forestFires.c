/*   Forest Fire (Simulation/Visualization)
 *   
 *   Summary: each tile simulates a single tree and lives happily unless
 *            struck by lightning or catching a spark of a neighboring fire
 *
 *   Rules: Based on Nicky Case's Simulating the World in Emoji
 *
 *   Dirt: % chance turning into a tree (10% in this case) to simulate growth
 *         Display dim color (emberlike)
 *   Tree: If neighbor is on fire, % chance turning into fire
 *         Display green     
 *   Fire: % chance turning into dirt (50% in this case) to simulate burning out (running out of fuel)
 *         Display red/orange
 *   Lightning: Occur on button press, turn tree to fire
 *         Display a flash or flashes of bright white
 *
 *   by Jonathan Bobrow
 *   01.2016
 */

#include "AutomaTiles.h"

uint8_t neighbors[6];
uint8_t colors[4][3] = {{8,4,0},         // Dead Tree
                         {26,255,0},       // Alive Tree
                         {255,128,8},     // Fire
                         {204,204,255}};   // Lightning
uint8_t darkColor[3] = {0,0,0};

uint8_t bLightning = 0;
uint16_t lightningDelay = 1500;
uint32_t lightningStrikeTime = 0;
uint32_t currentTime = 0;

void button() {
     // cause lighting
     bLightning = 1;
     lightningStrikeTime = currentTime;
}

void step() {
     // check neighbors
     getNeighborStates(neighbors);
     
     switch(getState()) {
          case 0: 
               // we are dirt, become a tree occasionally 
               // higher probability with other trees around
               for(uint8_t i=0; i<6; i++) {
                    if(neighbors[i] == 1 && currentTime % 6 == i) {
                         setState(1);
                    }
               }
               // if no trees around, 10%
               if(currentTime % 10 == 0) {
                    setState(1);
               }
               break;
          case 1: 
               for(uint8_t i=0; i<6; i++) {
                    if(neighbors[i] == 2){
                         setState(2);   // catch on fire
                         break;
                    }
               }
               break;
          case 2: 
               if(currentTime % 2 ==0)
                    setState(0);   // burn out with 50% probability
               break;
          default: break;
     }
     setColor(colors[getState()]);
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
      
      // listen to step forward
     setMicOn();
      
     // Keep the processor running
     while(1) {
            currentTime = getTimer();
          // handle any continuous logic here
          if(bLightning) {
               if(currentTime - lightningStrikeTime >= lightningDelay) {
                    if(getState() != 0) {
                         setState(2);   // make fire
                    }
                    setColor(colors[getState()]);
                    bLightning = 0;
               }
               else if(currentTime%3 > 0) {     // this flashes the light during lightning
                    setColor(darkColor);
               }
               else {
                    setColor(colors[3]);
               }
          }
     }
}