var rad3 = Math.sqrt(3);
var EASEVAL = .1;

Cell = function (i) {
    this.index = i;

    this.side = settings.side_length;
    this.position = {x: 0, y: 0};
    this.targetPosition = {x: 0, y: 0};

    this.colorOn = settings.color_state_on;//'#ffffff';
    this.colorOff = settings.color_state_off;//'#000000';

    this.isHover = false;
    this.isHighlight = false;

    this.currentShape = 'hexagon';
    this.state = 0;
    this.nextState = 0;

    this.neighbors = [];

    // create the six points to be updated for each shape
    var points = [
        new Two.Anchor(0, 0),
        new Two.Anchor(0, 0),
        new Two.Anchor(0, 0),
        new Two.Anchor(0, 0),
        new Two.Anchor(0, 0),
        new Two.Anchor(0, 0)
    ];
    this.shape = two.makePolygon(points);
    this.shape.stroke = settings.color_stroke;
    this.shape.linewidth = 4;
    this.shape.fill = this.colorOff;

    this.targetVertices = [{x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}];
}

Cell.prototype = {
    init: function (i) {
        this.index = i;
        this.side = settings.side_length;
        this.position = {x: 0, y: 0};
        this.targetPosition = {x: 0, y: 0};

        this.colorOn = '#ffffff';
        this.colorOff = '#000000';

        this.currentShape = 'hexagon';
        this.state = 0;
        this.nextState = 0;

        this.neighbors = [];

        // create the six points to be updated for each shape
        var points = [
            new Two.Anchor(0, 0),
            new Two.Anchor(side, 0),
            new Two.Anchor(side * 3 / 2, side * rad3 / 2),
            new Two.Anchor(side, side * rad3),
            new Two.Anchor(0, side * rad3),
            new Two.Anchor(-side / 2, side * rad3 / 2)
        ];
        this.shape = two.makePolygon(points);
        this.shape.translation.set(300, 100);
        this.shape.stroke = '#ffffff';
        this.shape.fill = this.colorOff;

        this.targetVertices = [{x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}, {x: 0, y: 0}];
    },

    setSideLength: function (side_length) {
        this.side = side_length;
        this.setShape(this.currentShape);
    },

    setShape: function (shape) {

        switch (shape) {

            case 'hexagon':
                // determine the side length
                //this.side = 1300 / (COLS * 3);

                this.targetPosition.x = (this.index % COLS + (.5 * (Math.floor(this.index / COLS) % 2))) * this.side * 3;
                this.targetPosition.y = Math.floor(this.index / COLS) * this.side * rad3 / 2;

                // set vertices targets
                this.targetVertices[0] = {x: 0, y: 0};
                this.targetVertices[1] = {x: this.side, y: 0};
                this.targetVertices[2] = {x: this.side * 3 / 2, y: this.side * rad3 / 2};
                this.targetVertices[3] = {x: this.side, y: this.side * rad3};
                this.targetVertices[4] = {x: 0, y: this.side * rad3};
                this.targetVertices[5] = {x: -this.side / 2, y: this.side * rad3 / 2};

                break;

            case 'square':
                // determine the side length
                //this.side = 1000 / COLS;

                this.targetPosition.x = (this.index % COLS) * this.side;
                this.targetPosition.y = Math.floor(this.index / COLS) * this.side;

                // set vertices targets
                this.targetVertices[0] = {x: 0, y: 0};
                this.targetVertices[1] = {x: this.side, y: 0};
                this.targetVertices[2] = {x: this.side, y: this.side / 2};
                this.targetVertices[3] = {x: this.side, y: this.side};
                this.targetVertices[4] = {x: 0, y: this.side};
                this.targetVertices[5] = {x: 0, y: this.side / 2};

                // console.log("target position x: " + this.targetPosition.x + " y: " + this.targetPosition.y);

                break;

            case 'triangle':
                // determine the side length
                //this.side = 1000 / COLS;

                var isArrowUp = this.index % 2;

                var oddRow = Math.floor(this.index / COLS) % 2;	// start the odd rows on the opposite of even rows

                if (oddRow == 0) {
                    isArrowUp += 1;
                    isArrowUp = isArrowUp % 2;
                }


                this.targetPosition.x = this.side * (this.index % COLS) / 2;
                this.targetPosition.y = Math.floor(this.index / COLS) / 2 * this.side * rad3;

                // set vertices targets
                if (isArrowUp == 0) {
                    this.targetVertices[0] = {x: 0, y: 0};
                    this.targetVertices[1] = {x: this.side / 2, y: 0};
                    this.targetVertices[2] = {x: this.side, y: 0};
                    this.targetVertices[3] = {x: this.side * 3 / 4, y: this.side * rad3 / 4};
                    this.targetVertices[4] = {x: this.side / 2, y: this.side * rad3 / 2};
                    this.targetVertices[5] = {x: this.side / 4, y: this.side * rad3 / 4};
                }
                else {
                    this.targetVertices[0] = {x: this.side / 2, y: 0};
                    this.targetVertices[1] = {x: this.side / 2, y: 0};
                    this.targetVertices[2] = {x: this.side, y: this.side * rad3 / 2};
                    this.targetVertices[3] = {x: this.side, y: this.side * rad3 / 2};
                    this.targetVertices[4] = {x: 0, y: this.side * rad3 / 2};
                    this.targetVertices[5] = {x: 0, y: this.side * rad3 / 2};
                }

                break;
        }
        // set current shape
        this.currentShape = shape;

        // get the new neighbors
        this.getNeighbors();
    },

    // TODO: figure out wrapping neighbors or ignore out of bound neighbors
    getNeighbors: function () {

        // create temp neighbors
        var tempNeighbors = [];

        switch (this.currentShape) {

            case 'triangle':
                // this gets a bit busy, so handle it in its own func :)
                tempNeighbors = this.getTriangleNeighbors();
                break;

            case 'square':
                // top left
                tempNeighbors.push(this.index - COLS - 1);
                // top
                tempNeighbors.push(this.index - COLS);
                // top right
                tempNeighbors.push(this.index - COLS + 1);
                // right
                tempNeighbors.push(this.index + 1);
                // bottom right
                tempNeighbors.push(this.index + COLS + 1);
                // bottom
                tempNeighbors.push(this.index + COLS);
                // bottom left
                tempNeighbors.push(this.index + COLS - 1);
                // left
                tempNeighbors.push(this.index - 1);
                break;

            case 'hexagon':
                if (Math.floor(this.index / COLS) % 2 == 0) {
                    // top left
                    tempNeighbors.push(this.index - COLS - 1);
                    // top
                    tempNeighbors.push(this.index - 2 * COLS);
                    // top right
                    tempNeighbors.push(this.index - COLS);
                    // bottom right
                    tempNeighbors.push(this.index + COLS);
                    // bottom
                    tempNeighbors.push(this.index + 2 * COLS);
                    // bottom left
                    tempNeighbors.push(this.index + COLS - 1);
                }
                else {
                    // top left
                    tempNeighbors.push(this.index - COLS);
                    // top
                    tempNeighbors.push(this.index - 2 * COLS);
                    // top right
                    tempNeighbors.push(this.index - COLS + 1);
                    // bottom right
                    tempNeighbors.push(this.index + COLS);
                    // bottom
                    tempNeighbors.push(this.index + 2 * COLS);
                    // bottom left
                    tempNeighbors.push(this.index + COLS + 1);
                }
                break;
        }

        // replace neighbors with temp neighbors
        this.neighbors = tempNeighbors.slice();

        return this.neighbors;

        // console.log("neighbors: ");
        // console.log(this.neighbors);
    },

    getTriangleNeighbors: function () {
        // create temp neighbors
        var tempNeighbors = [];

        var row = Math.floor(this.index / COLS);

        switch (row % 4) {
            case 0:
                tempNeighbors.push(this.index + COLS);
                tempNeighbors.push(this.index + COLS - 1);
                tempNeighbors.push(this.index - COLS);
                break;

            case 1:
                tempNeighbors.push(this.index - COLS);
                tempNeighbors.push(this.index - COLS - 1);
                tempNeighbors.push(this.index + COLS - 1);
                break;

            case 2:
                tempNeighbors.push(this.index + COLS);
                tempNeighbors.push(this.index + COLS - 1);
                tempNeighbors.push(this.index - COLS - 1);
                break;

            case 3:
                tempNeighbors.push(this.index - COLS);
                tempNeighbors.push(this.index - COLS - 1);
                tempNeighbors.push(this.index + COLS);
                break;
        }

        // replace neighbors with temp neighbors
        return tempNeighbors;

    },

    setColorOn: function (color) {
        this.colorOn = color;
    },

    setColorOff: function (color) {
        this.colorOff = color;
    },

    setFill: function (color) {
        this.shape.fill = color;
    },

    setStroke: function (color) {
        this.shape.stroke = color;
    },

    setState: function (state) {
        this.state = state;
    },

    setNextState: function (state) {
        this.nextState = state;
    },

    update: function () {
        // update the position of the shape
        // console.log("target position x: " + this.targetPosition.x);
        // console.log("actual position x: " + this.position.x);

        if (Math.abs(this.targetPosition.x - this.position.x) > 0.5 || Math.abs(this.targetPosition.y - this.position.y) > 0.5) {
            this.position.x = this.position.x + EASEVAL * (this.targetPosition.x - this.position.x);
            this.position.y = this.position.y + EASEVAL * (this.targetPosition.y - this.position.y);

            this.shape.translation.set(this.position.x, this.position.y);
        }
        else if (Math.abs(this.targetPosition.x - this.position.x) > 0.0 || Math.abs(this.targetPosition.y - this.position.y) > 0.0) {
            this.position.x = this.targetPosition.x;
            this.position.y = this.targetPosition.y;

            this.shape.translation.set(this.position.x, this.position.y);
        }

        // update the position of vertices
        for (var i = 0; i < 6; i++) {
            if (Math.abs(this.targetVertices[i].x - this.shape.vertices[i].x) > 0.5)
                this.shape.vertices[i].x = this.shape.vertices[i].x + EASEVAL * (this.targetVertices[i].x - this.shape.vertices[i].x);
            else if (Math.abs(this.targetVertices[i].x - this.shape.vertices[i].x) > 0.0)
                this.shape.vertices[i].x = this.targetVertices[i].x;

            if (Math.abs(this.targetVertices[i].y - this.shape.vertices[i].y) > 0.5)
                this.shape.vertices[i].y = this.shape.vertices[i].y + EASEVAL * (this.targetVertices[i].y - this.shape.vertices[i].y);
            else if (Math.abs(this.targetVertices[i].y - this.shape.vertices[i].y) > 0.0)
                this.shape.vertices[i].y = this.targetVertices[i].y;
        }

        // update state of on or off in simulation
        if (this.isHover == true) {
            this.shape.fill = settings.color_hover;
        }
        else if (this.isHighlight == true) {
            this.shape.fill = settings.color_neighbor;
        }
        else if (this.state == 0) {
            this.shape.fill = this.colorOff;
        }
        else if (this.state == 1) {
            this.shape.fill = this.colorOn;
        }

    },

    draw: function () {

    }

}