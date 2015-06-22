var two;
var population = [];

var ROWS = 20;
var COLS = 10;


// reshape the population
var makeTri = function(){
  for(var i=0; i<ROWS*COLS; i++) {
    population[i].setShape('triangle');
  }
}

var makeSquare = function(){
  for(var i=0; i<ROWS*COLS; i++) {
    population[i].setShape('square');
  }
}

var makeHex = function(){
  for(var i=0; i<ROWS*COLS; i++) {  
    population[i].setShape('hexagon');
  }
}


// determine the next state for the population
var simulate = function(){

  var populationBuffer = [];
  // Save cells to buffer (so we opeate with one array keeping the other intact)
  populationBuffer = population.splice();
  
  // Visit each cell:
  for(var i=0; i<ROWS*COLS; i++) {
    var neigbors = 0;

  }

  // And visit all the neighbours of each cell
  // We'll count the neighbours
  // Check alive neighbours and count them
  // End of yy loop
  // End of xx loop
  // We've checked the neigbours: apply rules!
  // The cell is alive: kill it if necessary
  // Die unless it has 2 or 3 neighbours
  // The cell is dead: make it live if necessary      
  // Only if it has 3 neighbours
  // End of y loop
  // End of x loop
}



// Do this stuff on load (thanks jquery!)
$(function() {

  two = new Two({ 
      width:$( window ).width(),
      height:$( window ).height(),
      fullscreen: false
  });
 
  two.appendTo(document.getElementById("two")); 


  for(var i=0; i<ROWS*COLS; i++) {
    population.push(new Cell(i));
    population[i].setShape('square');
  }

  // Update the renderer in order to generate corresponding DOM Elements.
  two.update();

  // Add touch events to the cells
  for(var i=0; i<ROWS*COLS; i++) {
    $(population[i].shape._renderer.elem)
      .css('cursor', 'pointer')
      .click(function(e) {
        // console.log("I JUST TOUCHED SHAPE!!!!");
        var idx = e.toElement.id.split("_").pop() - 1;
        // console.log(idx);
        population[idx].setFill('#000000');
    });
  }

  _.defer(function() {

    two.bind('resize', function() {

      })
      .bind('update', function(frameCount) {

        // update loop here
        
        for(var i=0; i<ROWS*COLS; i++) {
          population[i].update();
        }

      })
      .play();
  
  });

});