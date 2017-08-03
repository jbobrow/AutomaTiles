
uint8_t speedMultiplyer = 20;

uint8_t empty = 10;
uint8_t passBall = 1;
uint8_t hasBall = 2;
uint8_t hadBall = 3;
uint8_t dead = 4;

uint8_t lonelyCounterThreshold = 20;
uint8_t lonelyCounter;
uint8_t neighborhoodValue = 1;
boolean wasAlone = false;

uint8_t ball[3] = {255, 255, 255};
uint8_t passing[3] = {150, 150, 200};
uint8_t darkWhite[3] = {50, 50, 50};
uint8_t red[3] = {250, 0, 0};
uint8_t yellow[3] = {255, 255, 0};
uint8_t orange[3] = {200, 100, 0};
uint8_t blank[3] = {0, 0, 0};
uint8_t blue[3] = {0, 0, 255};
uint8_t green[3] = {0, 255, 0};
uint8_t neighbors[6];

uint32_t lastHadBall = 0;
uint32_t lastPassedBall = 0;
uint32_t gettingBall = 0;
uint32_t deadTime = 0;
uint32_t timeTimer = 0;

void setup() {
  setTimeout( 60000 );
  setState( empty );
  setColor( green );
  setButtonCallback( add_ball );
  setLongButtonCallback( trigger_end_game, 5000 );
  timeTimer = getTimer();
}

void trigger_end_game() {
  setState( dead );
  setColor( red );
  deadTime = getTimer();
}

void add_ball() {
  setState( hasBall );
  setColor( ball );
  lastHadBall = getTimer();
}

void loop() { 
  neighborhoodValue = 0;
  
  update_time_dilation();
  if ( getState() == dead ) {
    if ( getTimer() - deadTime > 400 && getTimer() - deadTime < 600 ) {
      setColor( orange );
    }
    else if ( getTimer() - deadTime > 600 && getTimer() - deadTime < 800 ) {
      setColor( yellow );
    }
    else if ( getTimer() - deadTime > 800 ) {
      setState( empty );
      setColor( green );
    }
  } else {
    if ( getState() == hasBall && (  getTimer() - lastHadBall > ( 200 * speedMultiplyer ) ) ) {
      setState( passBall );
      setColor( passing );
    }
    else if ( getState() == hadBall && ( getTimer() - lastPassedBall > ( 600 * speedMultiplyer ) ) ) {
      setState( empty );
      setColor( green );
    }

    getNeighborStates( neighbors );       
    
    for ( uint8_t i = 0; i < 6; i++ ) {
      neighborhoodValue += neighbors[ i ];
      if ( neighbors[ i ] == dead ) {
        trigger_end_game();
        return;
      }
      if ( getState() == empty && neighbors[ i ] == passBall ) {
        if (gettingBall == 0 ) {
          gettingBall = getTimer();
        } else if ( getTimer() - gettingBall >  ( 100 * speedMultiplyer ) ) {
          gettingBall = 0;
          setState( hasBall );
          setColor( ball );
          lastHadBall = getTimer();
        }
      } else if ( getState() == passBall && neighbors[ i ] == hasBall ) {
        setState( hadBall );
        setColor( darkWhite );
        lastHadBall = 0;
        lastPassedBall = getTimer();
      } else if ( getState() == passBall && getTimer() - lastHadBall > ( 500 * speedMultiplyer ) ) {
        trigger_end_game();
      }
    }
    if ( neighborhoodValue > 0  ) {
      lonelyCounter = 0;  
    }
    if ( wasAlone && neighborhoodValue > 0 && getState() == empty ) {           
      wasAlone = false; 
      randomSeed( getTimer() );
      if ( random( 0, 50 ) < 15 ) {
        setState( 11 );
        setColor( blank );
      } else {
        setColor( green );
      }
    } else if ( neighborhoodValue == 0 ) {
      lonelyCounter++;      
      if ( lonelyCounter >= lonelyCounterThreshold ) {
          lonelyCounter = lonelyCounterThreshold;
          wasAlone = true;
          if ( getState() == 11 ) {
            setState( empty );
            setColor( green );
          }
      }      
    }   
  }
}

void update_time_dilation() {
  if ( getTimer() - timeTimer > ( 1000 * speedMultiplyer ) ) {
    timeTimer = getTimer();
    speedMultiplyer -= 1;
    if ( speedMultiplyer < 1 ) {
      speedMultiplyer = 1;
    }
  }
}
//uint8_t all( uint8_t *arr, uint8_t value ) { // The results come out reversed
//  for( uint8_t i = 0; i < sizeof(arr)/sizeof(arr[0]); i++ ) {
//    if ( arr[ i ] != value ) {
//      return 0;  
//    }
//  }
//  return 1;
//}

//uint8_t test_no_where_to_go() { // The results come out reversed
//  for ( uint8_t i = 0; i < 6; i++ ) {
//     if ( neighbors[ i ] == empty ) {
//         return 0;         
//     }
//  }
//  return 1;    
//}
