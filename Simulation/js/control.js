var settings = new Settings();

function Settings() {
    this.duration = 120;
    this.side_length = 20;

    this.color_state_on = '#ffffff';
    this.color_state_off = '#333333';
    this.color_stroke = '#ffffff';

    this.highlight = true;
    this.color_hover_on = '#ffcdbf';
    this.color_neighbor_on = '#f8e8e2';
    this.color_hover_off = '#ff6d58';
    this.color_neighbor_off = '#6f4c49';

    this.shape = 'hexagon';

    this.frequency = 2;

    this.autoplay = false;

    this.step = function () {
        //step forward in game of life
        //step();
        simulate();
    };

    this.reset = function () {
        // reset the board to blank
        reset();
    };

    this.save = function () {
        var container = document.querySelector('#two');
        window.open('data:image/svg+xml,' + container.innerHTML);
    }
}

/* visible control panel */
var gui = new dat.GUI();

// place a dropdown here for shapes
var shape_control = gui.add(settings, 'shape', ['triangle', 'square', 'hexagon']);
var side_length_control = gui.add(settings, 'side_length', 2, 200).step(1);

gui.add(settings, 'step');

var autoplay_control = gui.add(settings, 'autoplay').listen();

var freq_control = gui.add(settings, 'frequency', 1, 20).step(1);

gui.add(settings, 'reset');

var f1 = gui.addFolder('color');
var color_state_on_control = f1.addColor(settings, 'color_state_on');
var color_state_off_control = f1.addColor(settings, 'color_state_off');
var color_stroke_control = f1.addColor(settings, 'color_stroke');
f1.closed = true;

var f2 = gui.addFolder('neighbors');
var highlight_control = f2.add(settings, 'highlight');
var color_hover_on_control = f2.addColor(settings, 'color_hover_on');
var color_neighbor_on_control = f2.addColor(settings, 'color_neighbor_on');
var color_hover_off_control = f2.addColor(settings, 'color_hover_off');
var color_neighbor_off_control = f2.addColor(settings, 'color_neighbor_off');
f2.closed = true;

gui.add(settings, 'save');

// handle color change
color_state_on_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        // population[i].setFill(settings.color_state_on);	// get rid of this after simulation running
        population[i].setColorOn(settings.color_state_on);
    }
});

color_state_off_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        // population[i].setFill(settings.color_state_off);	// get rid of this after simulation running
        population[i].setColorOff(settings.color_state_off);
    }
});

color_stroke_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setStroke(settings.color_stroke);
    }
});

color_hover_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        //population[i].setColorOff(settings.color_state_off);
    }
});

color_neighbor_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        //population[i].setStroke(settings.color_stroke);
    }
});

side_length_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setSideLength(settings.side_length);
    }
});

shape_control.onChange(function () {

    // pause the autoplay
    settings.autoplay = false;

    switch (settings.shape) {
        case 'triangle':
            makeTri();
            break;
        case 'square':
            makeSquare();
            break;
        case 'hexagon':
            makeHex();
            break;
    }
});

autoplay_control.onChange(function () {
    // turn autoplay on or off depending
});