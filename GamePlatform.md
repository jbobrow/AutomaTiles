#AutomaTiles as Game Platform

AutomaTiles allow a new type of game play as well as game design, and the following document will give a quick overview as to how to design for AutomaTiles and what their unique capabilities are.

##Overview

First of all, what are AutomaTiles? Each AutomaTile is a "smart" hexagonal tile, designed to be held in the hand or placed next to other tiles on a table. Magnets inside of each AutomaTile allow the hexagonal tile to maintain an organized gridlike structure without needing a board. Each tile has 3 properties:
1. it responds to presses (this is a tactile button press, the entire tile depresses ever so slightly, so players can feel when they have pressed on a tile, not simply touched)
2. it is social, which means it is aware of its neighbors, tiles on other sides, both presence as well as state (explained further below)
3. it is rythmic, can respond to sound (currently treated as a global message, for example a time step forward)

##Outputs

AutomaTiles communicate to the players through RGB illumination and their hue, saturation, brightness, as well as animated properties (i.e. pulsing, blinking, flashing...). Their is only a single uniform illumination for the entire tile, so think of each tile as being able to communicate relatively simple information.
In one example, a tile shows it is in a win condition by blinking rapidly, so all players are aware of the game state.

While illumination is how a single tile communicates to the players, the arrangement of the tiles also communicates something to the players. In one example, the arrangement of 10 tiles into different forms creates different behaviours of the whole. For example, if the tiles are representing trees in a forest fire, a tightly packed set of 10 will spread the fire quicker than a long single file line of tiles(trees). The player can reason this by seeing the arrangement of the board, and conversely, affect it by rearranging the board. Which brings me to inputs.

##Inputs

The simplest of inputs is a press. Each tile acts as a mechanical momentary button. The button can be used in a variety of ways, and can respond to a quick press, a long press, a double press...

The button press can be an input to change the state of a tile, or inform the tile of a users action. For example, in the forest fire ruleset, a single press strikes lightning upon a tile. If the tile is a tree, it will then change state to be on fire, and if it is not a tree, it will simply go on being not a tree, as well as not on fire.

The press can also be used to reveal information contained in a tile. For example, a tile representing a place in a game of minesweeper could reveal that it has a mine under it only when held with a long press. The button press in this case didn't add information to the system but is simply used to reveal information.

##Tile Communication

The tiles communicate with each of their six tile neighbors by receiving information every ~20 times per second. The information passed is in the form of a single integer (0-16). If no tile is present on a single side, the tile reads 0 on that side. (A tile can be present and send 0 as well, but it will be treated the same as space without a tile). What does this mean in simple terms? This means that each tile can tell each neighboring tile that it is in one of roughly 15 states. In the forest fire example, there are 3 states: tree, fire, soil. But what about lightning? that isn't a state, since it only affects the local tile, so it is animated through the illumination, but simply changes the tile's state from tree to fire.

AutomaTiles are setup to do this kind of communication often and reliably, but it is not the only way they can communicate. For longer forms of information, such as passing string-like information, there is a shared data buffer that could be utilized. This is experimental and not refined in the API, so I recommend first trying to rely on simple communication with the dozen or so states afforded above.

##Time

Game time for the tiles can be continuous, evaluating their state 20 times a second, or they can be discrete, evaluating their state only when a step is called. The step can be heard from the microphone or sensed via a pacemaker(special AutomaTile that silently flashes in the same way a metronome clicks).

In the forest fire example, time is discrete, which makes the spread of the fire controlled by the speed of the metronome. One could see this as a game mechanic, where each turn is a click or step forward.

In the game Fracture, a continuous game, the tiles are constantly evaluating their state so they can show players the most up to date representation of the current board state. This results in a board that is quickly responsive to arrangement, while discrete time makes a board that feels responsive to sound, pacemaker, or simply discrete time.

##Memory

While tiles can maintain a bit of a memory or history of what they have done, neighbors they've had, or how much time has passed, this is limited and it is important to remember that if this isn't transparent to the player, it will be very confusing. However, there will be elegant solutions in which a bit of stygmergy might just seep its way into a game and shed light on a beautiful invisible system that lies just beneath the surface.

##Trickiness

As you might have noticed, you have complete control over the way a tile displays itself to the player and the state a tile is actually in. It might be useful to have many states an only 2 visibly different appearances to the players. Conversely, it might be useful to have only two states and show many different illuminations.


The possibilities are many, feel free to look through the samples, they are only a few, but cover a couple of use cases.

Please feel free to ask about features, and if I didn't cover something that is in the API, I should, so point it out, it belongs here, too!