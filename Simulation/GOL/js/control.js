var settings = new Settings();

function Settings(){
    this.duration = 120;
    this.distance = 20;

    this.color_state_on = '#ff5555';
    this.color_state_off = '#550000';
    this.color_stroke = '#DF5B57';

    this.step = function(){
        //step forward in game of life
        step();
    };

    this.triangle = function(){
    	// make shape a triangle
    	makeTri();
    };

    this.square = function(){
    	// make shape a square
    	makeSquare();
    };

    this.hexagon = function(){
    	// make shape a hexagon
    	makeHex();
    };
}

/* visible control panel */
var gui = new dat.GUI();

var f0 = gui.addFolder('actions');
f0.add(settings, 'step');
f0.closed = false;

var f1 = gui.addFolder('colors');
var color_state_on_control = f1.addColor(settings, 'color_state_on');
var color_state_off_control = f1.addColor(settings, 'color_state_off');
var color_stroke_control = f1.addColor(settings, 'color_stroke');
f1.closed = false;

// place a dropdown here for shapes
var f2 = gui.addFolder('shapes');
f2.add(settings, 'triangle');
f2.add(settings, 'square');
f2.add(settings, 'hexagon');
f2.closed = false;

// handle color change
color_state_on_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
      population[i].setFill(settings.color_state_on);
    }

});

color_state_off_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
      population[i].setFill(settings.color_state_on);
    }
});

color_stroke_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
      population[i].setStroke(settings.color_stroke);
    }
});
