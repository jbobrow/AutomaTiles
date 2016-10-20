/*  Puzzle of Fire and Ice (Game)
 *  
 *  Setup: 1 or 2 player game. Rubik's cube like in that the tiles are arranged in a solved state, set,
 *         and then the puzzle is messed up with a goal of finding the solved arrangement
 *  
 *  Technical Details:
 *    Press button to set the solution array (i.e. on press, record where neighbors are present and save to array)
 * 
 *    The tiles need to first determine if they themselves are solved,
 *    then through a back and forth with neighboring tiles, determine
 *    if their neighbors are solved and only celebrate if that is verified
 *    across the entire network of tiles
 *    
 *    States for game piece
 *    0 = no piece (never in state 0)
 *    1 = present and unsolved
 *    2 = present and satisfied
 *    3 = present and satisfied and attached to satisfied piece
 *    4 = present and satisfied and attached to satisfied piece for n seconds (WIN STATE!!!) CELEBRATE
 * 
 *  Game devopment by: Nick Bently, Jonathan Bobrow
 *
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  10.07.2016
 */
 
uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 50;  //milliseconds

uint8_t closestNeighbor;
uint8_t numMovesAway;
uint8_t prevNumMovesAway;
uint8_t diffNumMovesAway;

uint8_t numNeighborsSolution = 0;
uint8_t numNeighbors;
uint8_t prevNumNeighbors;

uint8_t lowestNeighborState;
uint8_t bNeighborsSatisfied;
uint8_t bNeighborsSatisfiedWaiting;
uint8_t bNeighborsSolved;

uint32_t lastTimeColorChanged = 0;

uint8_t neighbors[6];
uint8_t prevNeighbors[6] = {255,255,255,255,255,255};

uint8_t solution[6];  // != 0 if present, 0 if absent

uint8_t warm[3] = {255,153,0};
uint8_t cold[3] = {0,153,253};
uint8_t neutral[3] = {255,255,255};

uint8_t colors[4][3] = {{255,0,0},     // red     (unsolved)
                        {255,255,0},   // yellow  (satisfied)
                        {0,255,0},     // green   (satisfied waiting for confirmation)
                        {0,255,255}};  // cyan    (solved)

// the color we will actually show
uint8_t outputColor[3];

// only update the state if the neighbors are confirmed to change
uint8_t didNeighborsChangeCount = 0;
uint8_t didNeighborsChangeCountThreshold = 10;

void setup() {
  setLongButtonCallback(button, 1000);  // setup a button handler (only for long press)
  setState(1);                          // set initial state
  setMicOff();                          // listen to step forward
  setTimeout(180);                      // 3 minutes
}

void loop() {
  
  uint32_t curTime = getTimer();
 
  if(curTime - lastUpdateTime > updateFrequency) {

    // read the values from our neighbors
    getNeighborStates(neighbors);

    // check if we have a different neighbor
    for(uint8_t i=0; i<6; i++) {
      if(neighbors[i] != prevNeighbors[i]) {
        didNeighborsChangeCount++;
        break;
      }

      if(i==5) {
        didNeighborsChangeCount = 0;
      }
    }

    // if the surroundings changed
    // (10 times in a row, redundant checking helps with stability)
    if(didNeighborsChangeCount >= 10) {

      // reset count
      didNeighborsChangeCount = 0;
      
      // Check to see if we are in a better or worse position than our previous position
      // We calculate better/worse based on two variables: 
      // accuracy for number of neighbors, and accuracy of position
      //
      // 1. Check to see if we have the right number of neighbors
      numNeighbors = 0;
      prevNumNeighbors = 0;
      for(uint8_t i=0; i<6; i++) {
        // count neighbors
        if(neighbors[i] != 0) {
          numNeighbors++;
        }
        if(prevNeighbors[i] != 0) {
          prevNumNeighbors++;
        }
      }
            
      // 2. Check to see how far away those neighbors are
      numMovesAway = 0;
      bNeighborsSatisfied = 0;
      bNeighborsSatisfiedWaiting = 0;
      bNeighborsSolved = 0;
      lowestNeighborState = 4;  // start at highest possible

      // calculate how far we are from a solution state
      for(uint8_t i=0; i<6; i++) {
        
        // if solution needs a neighbor
        closestNeighbor = 6;
        if(solution[i] != 0) {
          
          // look through all neighbors and find closest one present
          for(uint8_t j=0; j<6; j++) {

            // is neighbor present
            if(neighbors[j] != 0) {

              // set lowest neighbor state
              if(neighbors[j] < lowestNeighborState) {
                lowestNeighborState = neighbors[j];
              }
                
              // absolute distance from solution (remember, this is distance on a ring)
              if(abs(j-i-6) % 6 < closestNeighbor) {
                closestNeighbor = abs(j-i-6) % 6; 
              }
            }
          }
          // total the moves away for each solution spot that needs to be satisfied
          numMovesAway += closestNeighbor;
        }
        // return to check the next solution spot
      }

      // are my neighbors solved...
      if(lowestNeighborState == 2) {
        bNeighborsSatisfied = 1;
      }
      if(lowestNeighborState == 3) {
        bNeighborsSatisfiedWaiting = 1;
      }
      if(lowestNeighborState == 4) {
        bNeighborsSolved = 1;
      }
      
      // calculate difference in moves away from before
      diffNumMovesAway = numMovesAway - prevNumMovesAway;
      // a positive(+) value is worse
      // a negative(-) value is better

      // decide what to display based on position
      // if satisfied, change state
      if(numMovesAway == 0) {
        if(bNeighborsSatisfied) {
          setState(3);
          setColor(colors[2]);  // green  (satisfied and waiting)
        }
        else if(bNeighborsSatisfiedWaiting) {
          setState(4);
          setColor(colors[3]);  // cyan (solved)
        }
        else if(bNeighborsSolved) {
          // do something here to celebrate :)
        }
        else {
          setState(2);
          setColor(colors[1]);  // yellow  (satisfied)
        }
      }
      else {
        setState(1);
        setColor(colors[0]);  // red (unsatisfied)
      }
      // TODO: Determine weighting of number of neighbors to position of neighbors
      // if closer to solved, signal closer (warmer)
      // if further from solved, signal further (colder)

      // update prev neighbors
      for(uint8_t i=0; i<6; i++) {
        prevNeighbors[i] = neighbors[i];
      }

      // update prev number of moves away
      prevNumMovesAway = numMovesAway;
    }
    
    // update our update time
    lastUpdateTime = curTime;
  }
}

void button() {
  // when button is pressed, save the state of neighbors and set to solution array
  numNeighborsSolution = 0;
  
  // read the values from our neighbors
  getNeighborStates(neighbors);

  for(uint8_t i=0; i<6; i++) {
    solution[i] = neighbors[i];
    if(solution[i] != 0) {
      numNeighborsSolution++;
    }
  }
}
