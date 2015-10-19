var settings = new Settings();

function Settings() {
    this.duration = 120;
    this.side_length = 20;
    this.stroke_width = 4;

    this.color_state_on = '#ff0000';
    this.color_state_off = '#333333';
    this.color_stroke = '#ffffff';

    this.highlight = false;
    this.showNeighbors = false;
    this.color_hover_on = '#c000ff';
    this.color_neighbor_on = '#ff00d3';
    this.color_hover_off = '#2353a4';
    this.color_neighbor_off = '#3e4b67';

    this.shape = 'hexagon';

    this.frequency = 2;

    this.autoplay = false;

    this.isAnimated = true;

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

    this.allPresent = function () {
        // make all tiles present
        makeAllPresent();
    };

    this.allNotPresent = function () {
        // make all tiles not present
        makeAllNotPresent();
    };

    this.randomSelect = function () {
        // make all tiles not present
        selectRandomTiles();
    };
}

/* visible control panel */
var gui = new dat.GUI();

gui.add(settings, 'step');

var autoplay_control = gui.add(settings, 'autoplay').listen();

var freq_control = gui.add(settings, 'frequency', 1, 20).step(1).listen();

gui.add(settings, 'reset');

// place a dropdown here for shapes
var f0 = gui.addFolder('form');
var shape_control = f0.add(settings, 'shape', ['triangle', 'square', 'hexagon']);
var animate_control = f0.add(settings, 'isAnimated');
var side_length_control = f0.add(settings, 'side_length', 2, 200).step(1);
var stroke_width_control = f0.add(settings, 'stroke_width', 0, 10).step(1);
f0.closed = true;

var f1 = gui.addFolder('color');
var color_state_on_control = f1.addColor(settings, 'color_state_on');
var color_state_off_control = f1.addColor(settings, 'color_state_off');
var color_stroke_control = f1.addColor(settings, 'color_stroke');
f1.closed = true;

var f2 = gui.addFolder('neighbors');
var highlight_control = f2.add(settings, 'highlight');
var showNeighbors_control = f2.add(settings, 'showNeighbors');
var color_hover_on_control = f2.addColor(settings, 'color_hover_on');
var color_neighbor_on_control = f2.addColor(settings, 'color_neighbor_on');
var color_hover_off_control = f2.addColor(settings, 'color_hover_off');
var color_neighbor_off_control = f2.addColor(settings, 'color_neighbor_off');
f2.closed = true;

var f3 = gui.addFolder('advanced');
f3.add(settings, 'allPresent');
f3.add(settings, 'allNotPresent');
f3.add(settings, 'randomSelect');
f3.closed = true;

//gui.add(settings, 'save');

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
    // update background
    document.getElementById("two").style.background = settings.color_stroke;
    document.body.style.background = settings.color_stroke;

    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setStroke(settings.color_stroke);
    }
});

side_length_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setSideLength(settings.side_length);
    }
});

stroke_width_control.onChange(function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setStrokeWidth(settings.stroke_width);
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