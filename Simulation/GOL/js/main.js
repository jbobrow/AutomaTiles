var two;
var population = [];

var ROWS = 20;
var COLS = 10;

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