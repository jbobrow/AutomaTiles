/*	Forest Fire (Simulation/Visualization)
 *	
 *	Summary: each tile simulates a single tree and lives happily unless
 *	         struck by lightning or catching a spark of a neighboring fire
 *
 *	Rules: Based on Nicky Case's Simulating the World in Emoji
 *
 *	Dirt: % chance turning into a tree (10% in this case) to simulate growth
 *	      Display dim color (emberlike)
 *	Tree: If neighbor is on fire, % chance turning into fire
 *	      Display green     
 *	Fire: % chance turning into dirt (50% in this case) to simulate burning out (running out of fuel)
 *	      Display red/orange
 *	Lightning: Occur on button press, turn tree to fire
 *	      Display a flash or flashes of bright white
 *
 *	by Jonathan Bobrow
 *	01.2016
 */

#include "AutomaTiles.h"

uint8_t neighbors[6];
uint8_t colors[4][3] = {{51,26,0},         // Dead Tree
                         {26,255,0},       // Alive Tree
                         {255,153,26},     // Fire
                         {204,204,255}};   // Lightning

uint8_t bLightning = 0;
uint16_t lightningDelay = 500;
uint32_t lightningStrikeTime = 0;

void button() {
     // cause lighting
     bLightning = 1;
     lightningStrikeTime = getTimer();
     setColor(colors[3]);
}

void step() {
     // check neighbors
     getNeighborStates(neighbors);
     
     switch(getState()) {
          case 0: 
               // we are dirt, become a tree occasionally 10%
               if(getTimer()%10==0)
                    setState(1);
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
               if(getTimer()%2==0)
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

     // Keep the processor running “
     while(1){
          // handle any continuous logic here
          if(bLightning && getTimer() - lightningStrikeTime >= lightningDelay) {
               if(getState() != 0) {
                    setState(2);   // make fire
               }
               setColor(colors[getState()]);
               bLightning = 0;
          }
     }
}
