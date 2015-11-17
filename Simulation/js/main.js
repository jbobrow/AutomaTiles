var two;
var population = [];
var isRulesetHidden = false;

var ROWS = 30;
var COLS = 15;

var prevStepTime = 0;

var birthRules = [false, false, true, true, false, false, false, false, false];
var deathRules = [true, true, false, false, true, true, true, true, true];

// reshape the population
var makeTri = function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setShape('triangle');
    }
};

var makeSquare = function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setShape('square');
    }
};

var makeHex = function () {
    for (var i = 0; i < ROWS * COLS; i++) {
        population[i].setShape('hexagon');
    }
};

// highlight cells
var hoverBegin = function (idx) {

    // highlight the hovered
    population[idx].isHover = true;

    // show the neighbors
    if (settings.showNeighbors) {
        for (var i = 0; i < population[idx].getNeighbors().length; i++) {
            var cell = population[idx];
            var neighborID = cell.neighbors[i];
            if (neighborID < 0 || neighborID >= ROWS * COLS) {
                continue;
            }
            var neighbor = population[neighborID];
            neighbor.isHighlight = true;
        }
    }
};

// end hover cells
var hoverEnd = function (idx) {

    // highlight the hovered
    population[idx].isHover = false;

    // show the neighbors
    if (settings.showNeighbors) {
        for (var i = 0; i < population[idx].getNeighbors().length; i++) {
            var cell = population[idx];
            var neighborID = cell.neighbors[i];
            if (neighborID < 0 || neighborID >= ROWS * COLS) {
                continue;
            }
            var neighbor = population[neighborID];
            neighbor.isHighlight = false;
        }
    }
};


var invert = function (idx) {

    var cell = population[idx];
    for (var i = 0; i < cell.getNeighbors().length; i++) {
        var neighborID = cell.neighbors[i];
        if (neighborID < 0 || neighborID >= ROWS * COLS) {
            continue;
        } else if (!population[neighborID].isPresent) {
            continue;
        }

        population[neighborID].state = (population[neighborID].state + 1) % 2;
    }
};

// determine the next state for the population
var simulate = function () {

    // visit each cell:
    for (var i = 0; i < ROWS * COLS; i++) {

        // count cooperators
        var neighborsCooperating = 0;
        var numNeighbors = 0;

        //discuss with each neighbor
        for (var j = 0; j < population[i].getNeighbors().length; j++) {
            var cell = population[i];
            var neighborID = cell.neighbors[j];

            // check bounds
            if (neighborID < 0 || neighborID >= ROWS * COLS) {
                continue;
            } else if (!population[neighborID].isPresent) {
                continue;
            }

            // count number of present neighbors
            numNeighbors++;

            // negotiate w/ neighbor
            if (population[neighborID].state == 0)  // 0 state equals cooperate
                neighborsCooperating = neighborsCooperating + 1;
        }

        var payoffCoop = neighborsCooperating * settings.payoff_coop;
        var payoffDefect = (numNeighbors - neighborsCooperating) * settings.payoff_defect;

        if(payoffCoop >= payoffDefect) {
            // cooperate
            population[i].setNextState(0);
        }
        else {
            // defect
            population[i].setNextState(1);
        }
    }

    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        cell.setState(cell.nextState);
    }

    // And visit all the neighbours of each cell
    // We'll count the neighbours
    // Check alive neighbours and count them
    // We've checked the neigbours: apply rules!
    // The cell is alive: kill it if necessary
    // Die unless it has 2 or 3 neighbours
    // The cell is dead: make it live if necessary
    // Only if it has 3 neighbours
};

// reset the board to all off state
var reset = function () {
    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        cell.setState(0);
        cell.setNextState(0);
    }
};

// reset the board to all off state
var makeAllPresent = function () {
    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        cell.isPresent = true;
    }
};

// reset the board to all off state
var makeAllNotPresent = function () {
    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        cell.isPresent = false;
    }
};

// give each tile a 50% chance of being on
var selectRandomTiles = function () {
    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        var state = Math.random() > 0.5 ? 1 : 0;
        cell.setState(state);
        cell.setNextState(state);
    }
};


// Do this stuff on load (thanks jquery!)
$(function () {

    two = new Two({
        //width: $(window).width(),
        //height: $(window).height(),
        //type: Two.Types.canvas, // set the type of renderer // note: canvas on click needs to be handled differently
        fullscreen: true
    });

    two.appendTo(document.getElementById("two"));


    for (var i = 0; i < ROWS * COLS; i++) {
        population.push(new Cell(i));
        population[i].setShape('hexagon');
    }

    // Update the renderer in order to generate corresponding DOM Elements.
    two.update();

    // Add touch events to the cells
    for (var i = 0; i < ROWS * COLS; i++) {
        $(population[i].shape._renderer.elem)
            .css('cursor', 'pointer')
            .click(function (e) {
                // console.log("I JUST TOUCHED SHAPE!!!!");
                var idx = e.toElement.id.split("_").pop() - 1;
                // console.log(idx);
                // set the state to the opposite
                if (e.shiftKey) {
                    population[idx].isPresent = !population[idx].isPresent;
                }
                else if (e.altKey) {
                    population[idx].setState((population[idx].state + 1) % 2);
                    invert(idx);
                }
                else {
                    population[idx].setState((population[idx].state + 1) % 2);
                }
            })
            .mouseenter(function (e) {
                var idx = e.toElement.id.split("_").pop() - 1;
                hoverBegin(idx);
                //console.log("start hover: " + idx);
            })
            .mouseleave(function (e) {
                var idx = e.fromElement.id.split("_").pop() - 1;
                hoverEnd(idx);
                //console.log("end hover: " + idx);
            }
        );
    }

    _.defer(function () {

        two.bind('resize', function () {

        })
            .bind('update', function (frameCount) {

                var millis = Date.now();

                if (millis - prevStepTime >= 1000 / settings.frequency && settings.autoplay) {
                    simulate();
                    prevStepTime = millis;
                }
                // update loop here

                for (var i = 0; i < ROWS * COLS; i++) {
                    population[i].update();
                }

            })
            .play();

    });

});

// handle keypress
$(document).keydown(function (e) {
    //console.log(e.which);

    if (e.which === 32) {    // spacebar pressed
        settings.autoplay = !settings.autoplay;
    }
    else if (e.which === 38) {  // up arrow
        if(settings.frequency < 20)
            settings.frequency++;
    }
    else if (e.which === 40) {  // down arrow
        if(settings.frequency > 1)
            settings.frequency--;
    }
    else if (e.which === 39) {  // right arrow
        simulate(); // step forward once
    }
    else if (e.which == 37) {   // left arrow
        console.log("no undo feature here"); // need to store history to implement this (not trivial)
    }
    else if (e.which === 72) {    // 'h' or 'H' is pressed
        isRulesetHidden = !isRulesetHidden;

        if (isRulesetHidden)
            document.getElementById("ruleset").style.display = 'none';
        else
            document.getElementById("ruleset").style.display = 'block';
    }
});
