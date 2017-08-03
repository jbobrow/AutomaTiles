//Colors
uint8_t white[3] = {255,255,255};
uint8_t dimWhite[3] = {75,75,75};
uint8_t green[3] = {0, 255, 0};

uint8_t immuneCounter = 0;

//Neighbor states
uint8_t neighbors[6];
uint8_t previous_neighbors[6];

//Team Colors, the first color indicates a neutral state
uint8_t teams[3][3] = { { 0, 0, 0 }, { 255, 0, 0 }, { 0, 0, 255 } };
// Dark team colors for team selection and attack indication
uint8_t darkTeams[3][3] = { { 0, 0, 0 }, { 10, 0, 0 }, { 0, 0, 10 } };

uint8_t pastState = 0;

//Has the tile been saved from being turned
boolean saved = false;

//States
boolean reset_state = false;
boolean attacking = false;
boolean underAttack = false;

void setup() {
  setTimeout( 600000 );
  setState( 15 );
  setColor( teams[ 0 ] );
  setButtonCallback( on_button_down );
  setLongButtonCallback( reset_team_color, 4000  );
}

void on_button_down () {
  if ( reset_state ) {
    uint8_t teamState = getState() + 1;
    if ( teamState > 2 ) {
      teamState = 1;
    }
    setState( teamState );
    setColor( darkTeams[ teamState ] );
    setTimerCallbackTime( 5000 );
  }
  else if ( underAttack && getState() != 15 ) {    
    saved = true;
    setState( pastState );
    setColor( green );
    setTimerCallbackTime( 2000  );
  }
  else if ( getState() != 15 ){
    attacking = true;    
    setColor( darkTeams[ getState() ] );
    setState( getState() + 2 );    
    setTimerCallback( done_attacking, 2000 );
  }  
}

void done_attacking () {
  if ( getState() > 2 ) {
    setState( getState() - 2 );
    setColor( teams[ getState() ] );
    attacking = false;
  }
}

void reset_team_color () {    
  setState( 15 );
  setColor( dimWhite );
  reset_state = true;
  attacking = false;
  underAttack = false;
  setTimerCallback( lock_team_color, 5000  );
}

void lock_team_color() {
  reset_state = false;
  if ( getState() == 15 ) {
    setColor( white );
  } else {
    setColor( teams[ getState() ] );  
  }
  
}

void resolve_attack () {
  if ( saved ) {
      setState( pastState );
      // setColor( teams[ pastState ] );
      saved = false;
  } else {
    setColor( teams[ getState() ] );
  }
  underAttack = false;
}

void loop() {
  getNeighborStates( neighbors );
  for( uint8_t i = 0; i < 6; i++ ) {
      if ( !saved && neighbors[ i ] != previous_neighbors[ i ] && ( neighbors[ i ] == 3 || neighbors[ i ] == 4 )  ) {
          pastState = getState();        
          setState( neighbors[ i ] - 2 );
          if ( pastState < 3 ) {
            setColor( darkTeams[ neighbors[ i ] - 2 ] );
            underAttack = true;
            setTimerCallback( resolve_attack, 2000 );
          } else {
            setState( neighbors[ i ] - 2 );
            setColor( teams[ neighbors[ i ] - 2 ] );
          }
          break;
      }       
  } 
  for( uint8_t i = 0; i < 6; i++ ) {
    previous_neighbors[i] = neighbors[i];
  }
}
