var settings = new Settings();

function Settings(){
    this.duration = 120;
    this.distance = 20;

    this.color_state_on = '#ffffff';
    this.color_state_off = '#333333';
    this.color_stroke = '#ffffff';

    this.shape = 'hexagon';

    this.frequency = 2;

    this.autoplay = false;

    this.step = function(){
        //step forward in game of life
        //step();
        simulate();
    };

    this.reset = function(){
        // reset the board to blank
        reset();
    };

    this.save = function(){
        var container = document.querySelector('#two');
        window.open('data:image/svg+xml,' + container.innerHTML);
    }
}

/* visible control panel */
var gui = new dat.GUI();

// place a dropdown here for shapes
var shape_control = gui.add(settings, 'shape', [ 'triangle', 'square', 'hexagon' ] );

gui.add(settings, 'step');

var autoplay_control = gui.add(settings, 'autoplay').listen();

var freq_control = gui.add(settings, 'frequency', 1, 20).step(1);

gui.add(settings, 'reset');

var f1 = gui.addFolder('colors');
var color_state_on_control = f1.addColor(settings, 'color_state_on');
var color_state_off_control = f1.addColor(settings, 'color_state_off');
var color_stroke_control = f1.addColor(settings, 'color_stroke');
f1.closed = true;

gui.add(settings, 'save');

// handle color change
color_state_on_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
    	// population[i].setFill(settings.color_state_on);	// get rid of this after simulation running
    	population[i].setColorOn(settings.color_state_on);
    }

});

color_state_off_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
    	// population[i].setFill(settings.color_state_off);	// get rid of this after simulation running
    	population[i].setColorOff(settings.color_state_off); 
    }
});

color_stroke_control.onChange(function() {
    for(var i=0; i<ROWS*COLS; i++) {
      population[i].setStroke(settings.color_stroke);
    }
});

shape_control.onChange(function() {

    // pause the autoplay
    settings.autoplay = false;

	switch(settings.shape) {
		case 'triangle':    makeTri(); 		break;
		case 'square': 		makeSquare(); 	break;
		case 'hexagon':     makeHex(); 		break;
    };
});

autoplay_control.onChange(function() {
	// turn autoplay on or off depending
});