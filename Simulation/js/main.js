var two;
var population = [];

var ROWS = 30;
var COLS = 15;

var prevStepTime = 0;


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
        if (neighborsAlive == 3) {
            //if(populationBuffer)
            // birth
            // console.log("birth cell");
            population[i].setNextState(1);
        }
        else if (neighborsAlive >= 2 && neighborsAlive <= 3) {
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

// reset the board to all off state
var reset = function () {
    // update all cells to their next state
    for (var i = 0; i < ROWS * COLS; i++) {
        var cell = population[i];
        cell.setState(0);
        cell.setNextState(0);
    }
};


// Do this stuff on load (thanks jquery!)
$(function () {

    two = new Two({
        width: $(window).width(),
        height: $(window).height(),
        //type: Two.Types.canvas, // set the type of renderer // note: canvas on click needs to be handled differently
        fullscreen: false
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