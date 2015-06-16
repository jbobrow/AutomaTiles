var settings = new Settings();

function Settings(){
    this.duration = 120;
    this.distance = 20;

    this.color_state_on = '#ff5555';
    this.color_state_off = '#550000';

    this.step = function(){
        //step forward in game of life
        step();
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

var f2 = gui.addFolder('shapes');
// place a dropdown here for shapes

// handle color change
color_state_on_control.onChange(function() {

});

color_state_off_control.onChange(function() {

});