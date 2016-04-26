# AutomaTiles
AutomaTiles presents a new way to interact with cellular automata. In a physical form, each tile can be manipulated, and progresses based on an external metronome or user input. The same intimate knowledge a movie editor has with a film is shared with computation, as the user can control the speed at which the system evolves and handle individuals as they respond to input.

## Components
1. Virtual AutomaTiles
    1. [Automatiles.com/sim](http://automatiles.com/sim) (written by Jonathan Bobrow, rendered w/ Two.js)
    2. [Automatiles.com/sim/emoji](http://automatiles.com/sim/emoji) (modified version of Nicky Case's Emoji World Simulator)
2. Tangible AutomaTiles
    1. Hardware
        1. Electronics ([Components](https://docs.google.com/spreadsheets/d/1YpmN0rzX8dJSE3OYGx2io3yXud3R6zgf5WAVNdhyiFE))
        2. Printed Circuit Boards ([Rev. 3](http://automatiles.tumblr.com/post/133176987321/rev-3-boards-batch-of-100-automatiles-so-much-to))
        3. [Case Designs + Models](http://automatiles.tumblr.com/post/143452350711/automatiles-w-silicon-jackets-for-members-week)
    2. Firmware
        1. Registers, Pin mapping...
        2. Low Level code not accessible in API
    3. API
        1. [AutomaTile Library](Arduino)
        2. [Documentation](Examples/API.md)
        3. [Example Code](Examples)

##License
Copyright (c) 2015 MIT

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
