var two;
var population = [];

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
    if(settings.showNeighbors) {
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
    if(settings.showNeighbors) {
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

        population[neighborID].state = (population[neighborID].state+1)%2;
    }
};

// determine the next state for the population
var simulate = function () {

    // visit each cell:
    for (var i = 0; i < ROWS * COLS; i++) {

        // count neighbors
        var neighborsAlive = 0;
        for (var j = 0; j < population[i].getNeighbors().length; j++) {
            var cell = population[i];
            var neighborID = cell.neighbors[j];

            if (neighborID < 0 || neighborID >= ROWS * COLS) {
                continue;
            } else if (!population[neighborID].isPresent) {
                continue;
            }

            if (population[neighborID].state == 1)
                neighborsAlive = neighborsAlive + 1;
        }

        // enfore the rules
        if (doBirthAutomaTile(neighborsAlive, population[i].state)) {
            // birth
            // console.log("birth cell");
            population[i].setNextState(1);
        }
        else if (doKeepAliveAutomaTile(neighborsAlive, population[i].state)) {
            // live
            // console.log("keep this cell alive");
            population[i].setNextState(1);
        }
        else {
            // die
            // console.log("let cell die");
            population[i].setNextState(0);
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

var doBirthAutomaTile = function(neighbors, state) {
    if(state == 0) {
        if(birthRules[0] && neighbors == 0) return true;
        if(birthRules[1] && neighbors == 1) return true;
        if(birthRules[2] && neighbors == 2) return true;
        if(birthRules[3] && neighbors == 3) return true;
        if(birthRules[4] && neighbors == 4) return true;
        if(birthRules[5] && neighbors == 5) return true;
        if(birthRules[6] && neighbors == 6) return true;
        if(birthRules[7] && neighbors == 7) return true;
        if(birthRules[8] && neighbors == 8) return true;
    }
    return false;
};

var doKeepAliveAutomaTile = function(neighbors, state) {
    if(state == 1) {
        if(!deathRules[0] && neighbors == 0) return true;
        if(!deathRules[1] && neighbors == 1) return true;
        if(!deathRules[2] && neighbors == 2) return true;
        if(!deathRules[3] && neighbors == 3) return true;
        if(!deathRules[4] && neighbors == 4) return true;
        if(!deathRules[5] && neighbors == 5) return true;
        if(!deathRules[6] && neighbors == 6) return true;
        if(!deathRules[7] && neighbors == 7) return true;
        if(!deathRules[8] && neighbors == 8) return true;
    }
    return false;
};

var doKillAutomaTile = function(neighbors, state) {
    if(state == 1) {
        if(deathRules[0] && neighbors == 0) return true;
        if(deathRules[1] && neighbors == 1) return true;
        if(deathRules[2] && neighbors == 2) return true;
        if(deathRules[3] && neighbors == 3) return true;
        if(deathRules[4] && neighbors == 4) return true;
        if(deathRules[5] && neighbors == 5) return true;
        if(deathRules[6] && neighbors == 6) return true;
        if(deathRules[7] && neighbors == 7) return true;
        if(deathRules[8] && neighbors == 8) return true;
    }
    return false;
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
        var state = Math.random()>0.5 ? 1 : 0;
        cell.setState(state);
        cell.setNextState(state);
    }
};


// init checkboxes to ruleset
var initRuleset = function() {
    // set birth rules
    document.getElementById("birth_0").checked = birthRules[0];
    document.getElementById("birth_1").checked = birthRules[1];
    document.getElementById("birth_2").checked = birthRules[2];
    document.getElementById("birth_3").checked = birthRules[3];
    document.getElementById("birth_4").checked = birthRules[4];
    document.getElementById("birth_5").checked = birthRules[5];
    document.getElementById("birth_6").checked = birthRules[6];
    document.getElementById("birth_7").checked = birthRules[7];
    document.getElementById("birth_8").checked = birthRules[8];
    // set death rules
    document.getElementById("death_0").checked = deathRules[0];
    document.getElementById("death_1").checked = deathRules[1];
    document.getElementById("death_2").checked = deathRules[2];
    document.getElementById("death_3").checked = deathRules[3];
    document.getElementById("death_4").checked = deathRules[4];
    document.getElementById("death_5").checked = deathRules[5];
    document.getElementById("death_6").checked = deathRules[6];
    document.getElementById("death_7").checked = deathRules[7];
    document.getElementById("death_8").checked = deathRules[8];
};


// update ruleset
var updateRuleset = function() {
    // update birth rules
    birthRules[0] = document.getElementById("birth_0").checked;
    birthRules[1] = document.getElementById("birth_1").checked;
    birthRules[2] = document.getElementById("birth_2").checked;
    birthRules[3] = document.getElementById("birth_3").checked;
    birthRules[4] = document.getElementById("birth_4").checked;
    birthRules[5] = document.getElementById("birth_5").checked;
    birthRules[6] = document.getElementById("birth_6").checked;
    birthRules[7] = document.getElementById("birth_7").checked;
    birthRules[8] = document.getElementById("birth_8").checked;
    // update death rules
    deathRules[0] = document.getElementById("death_0").checked;
    deathRules[1] = document.getElementById("death_1").checked;
    deathRules[2] = document.getElementById("death_2").checked;
    deathRules[3] = document.getElementById("death_3").checked;
    deathRules[4] = document.getElementById("death_4").checked;
    deathRules[5] = document.getElementById("death_5").checked;
    deathRules[6] = document.getElementById("death_6").checked;
    deathRules[7] = document.getElementById("death_7").checked;
    deathRules[8] = document.getElementById("death_8").checked;
};

// Do this stuff on load (thanks jquery!)
$(function () {

    // init ruleset
    initRuleset();

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
    console.log(e.which);
    if(e.which === 32) {    // spacebar pressed
        settings.autoplay = !settings.autoplay;
    }
});
