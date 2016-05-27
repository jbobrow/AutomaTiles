##AutomaTiles Examples##
The following examples are written using the AutomaTiles API for the Arduino IDE.

###Instructions for using the Arduino IDE w/ AutomaTiles###
1. copy the `AutomaTiles` folder into your Arduino directory i.e. `~/Documents/Arduino/hardware/`
2. launch or quit and relaunch Arduino IDE
3. in the top menu, go to `Tools > Board` and select `AutomaTile`
4. open an example and press the checkmark to make sure it compiles
5. Use an ISP of your choice and program your AutomaTiles!

###Examples###
**Color Gossip**
An example to show how colors can transition smoothly between each other. In this aesthetic example, tiles pulse white when alone, and then share colors when together. Pressing on one changes its color, and surrounding tiles try to match the color while transitioning smoothly over 3 seconds. With a large population, the result is a flow of colors, it is up to chance which one will contaminate them all.  

**Game of Life**
Based on John Conway's [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), this example follows a simple set of rules to emerge some very complex behavior. Different AutomaTile arrangements create multiple static and dynamic equilibria

**Forest Fire**
A forest fire simulation based on Nicky Case's [Simulating the World in Emoji](ncase.me/simulating). Each tile represents a plot of land that can grow a tree, be struck by lightning, and catch on fire. Fires spread, Trees grow back, can you create the most resilient forest?

**Fracture**
A game that capitalizes on simple rules and thinking with your hands. Fracture is a competitive game for diversity. Each player has a color and they are trying to make all of their (6) tiles happy, represented by blinking. To make a tile happy, it needs to have at least 2 neighbors and only be touching colors different from itself. Players move by "fracturing" the board into 2 parts and reconfiguring it to make themselves more diverse and their opponents less diverse.

**PrototypeColors**
This is your best friend for prototyping any game. Tiles will shine brightly one a a predefined set of colors, in this case: red, orange, yellow, green, cyan, blue, magenta. If pressed the tile will switch to blink that color, or blink faster with another press. Holding down a tile for >1 sec goes to the next color and double pressing the tile <1 sec goes back to the previous color. With these easy deterministic rules that are not affected by neighbors, you can imagine how a game could work before having to program how it does work.

**Rock, Paper, Scissors**
A simple ruleset that results in a trippy chase of colors. Looks quite similar to a BZ-reaction.

**WireWorld**
A simple rulest based on [Wireworld](https://en.wikipedia.org/wiki/Wireworld), a cellular automata design of Brian Silverman in 1987. This ruleset simulates the flow of electrons through conductive wiring. Each electron is represented with a head and a tail for direction or flow of current, and spaces that aren't wires are fittingly considered insulation. The hexagonal ruleset requires a redefinition of logic gates (if all are possible)