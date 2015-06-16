var two;
var test = [];

var ROWS = 10;
var COLS = 10;

  var makeTri = function(){
    for(var i=0; i<ROWS*COLS; i++) {
      test[i].setShape('triangle');
    }
  }

  var makeSquare = function(){
    for(var i=0; i<ROWS*COLS; i++) {
      test[i].setShape('square');
    }
  }

  var makeHex = function(){
    for(var i=0; i<ROWS*COLS; i++) {  
      test[i].setShape('hexagon');
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
    test.push(new Cell(i));
    test[i].setShape('square');
  }

  _.defer(function() {

    two.bind('resize', function() {

      })
      .bind('update', function(frameCount) {

        // update loop here
        
        for(var i=0; i<ROWS*COLS; i++) {
          test[i].update();
        }

      })
      .play();

  });
});