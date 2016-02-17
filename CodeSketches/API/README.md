##AutomaTiles API##

**tileSetup**
```
void tileSetup(void); 
// Needs to be called once in main 
// sets up inputs, outputs, timers… pretty much anything one wants to do with an AutomaTile starts here
```
**getNeighborStates**
```
void getNeighborStates(uint8_t * result);
// pass it an array for the size of the neighborhood and it will return each neighbor’s state in that array. 
```
**sendStep**
```
void sendStep(void);
// step forward in time, simulate ruleset
// maximum frequency 10Hz
// these do not cue up, i.e. if you send step more frequently, steps will be dropped 
```
**getTimer**
```
uint32_t getTimer(void);
// returns a value of time in milliseconds (starts at 0 from battery in) 
```
**setColor**
```
void setColor(const uint8_t color[3]);
// send this function (R,G,B) 0-255, color will change on next cycle 
```
**setState**
```
void setState(uint8_t state);
// set the state of self 0-15 (limited due to communication of state and step frequency)
// invalid states are ignored, i.e. return without state change
// NOTE: empty spaces act as tiles of state 0 (treat this accordingly) 
```
**getState**
```
uint8_t getState(void);
// get’s our own current state, 0-15 
```
**setStepCallback**
```
void setStepCallback(cb_func cb); 
// pass your own function to setStepCallback, this function takes no arguments and returns nothing
// can be set once in the beginning of your application, but can also be hot-swapped during runtime
// defaults to do nothing (change this to default to flash red… or white) 
```
**setButtonCallback**
```
void setButtonCallback(cb_func cb);
// pass your own function to setButtonCallback, this function takes no arguments and returns nothing
// can be set once in the beginning of your application, but can also be hot-swapped during runtime
// defaults to do nothing (change this to default to flash blue)
// NOTE: you cannot delay in the button callback (i.e. timer will not update while you are in the callback) 
```
**setTimeout**
```
void setTimeout(uint8_t seconds);
// sets the amount of time before sleeping in seconds
// defaults to 20 seconds
// if you want the automatile to never sleep send it 0, this will run down your battery 
```
**setMicOn**
```
void setMicOn(void);
// turns the microphone on (i.e. listens for snaps) 
```
**setMicOff**
```
void setMicOff(void);
// turns the microphone off (i.e. only steps forward from neighbor notification) 
```

###A default blank sketch looks like the following:### 
```
#include “AutomaTiles.h"

void button() {}

void step() {}

int main(void) {      
   tileSetup(); 
   setButtonCallback(button); 
   setStepCallback(step); 
   while(1){}
}
```
