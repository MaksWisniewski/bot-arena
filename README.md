# *<b> Script `RUN.sh` Usage </b>*

## Interactive mode
This script provides the following options:

1. **Run Docker build command**: Builds a Docker image using the specified Dockerfile. <u>*This option is necessary to execute before any other option.*</u>
2. **Run Shell in Docker container**: Launches shell in a Docker container based on the built image.
3. **Compile C++ bots**: Compiles C++ bots located in the `cpp_bots` directory.
4. **Clean C++ bots**: Cleans up compiled C++ bot files.
5. **Run simulation**: Executes a simulation using the specified Python script.

To use the script, run it and choose the desired option by entering the corresponding number.

## Command line arguments

### `build-docker`, `--build-docker`
Used to build docker image.

### `compile`, `--compile`
Used to compile C++ bots. You can specify one or more bots to be compiled.
If you don't provide any specific bot name, all bots are built by default.

Example usage:
- compile all bots: `./RUN.sh compile`, it is equivalent to `./RUN.sh compile all`.
- compile specific bots: `./RUN.sh compile minmax mcts`.

### `clean`, `--clean`
Used to clean compiled object files of C++ bots (it doesn't remove linked bot executables from `bots` directory). It is equivalent to `make clean` run in `cpp_bots` directory.

### `select-economy`, `-e`
Used to select economy configuration used in simulations. The configuration files are stored in [`configs/economies`](configs/economies) directory.
After running `./RUN.sh -e` you will be prompted to select one of available economies.
You can also provide configuration name directly from command line by `--economy` option.

Example usage:
```
./RUN.sh ./RUN.sh select-economy --economy easy
```

### `run-simulation`, `--sim`
Used to run simulation. You can run `./RUN.sh --sim` and you will be prompted to provide necessary paramteres.
You can also provide simulation parameters directly from command line using below options:
- `--b1 <name>` - left bot name
- `--b1_args <args>` - command line arguments for left bot
- `--b2 <name>` - right bot name
- `--b2_args <args>` - command line arguments for right bot
- `--map <name>` - map name, map files are stored in `maps` directory
- `--log_name <name>` - name of file the simulation log is written to, logs are created in `logs` directory
- `--games <number>` - number of games to be played
- `--ready_timeout <seconds>`- time that bots have for preprocessing (in seconds)
- `--move_timeout <seconds>` - time for single move (in seconds)
- `--game_timeout <seconds>` - time for single game (in seconds)

Above options can be displayed by running `./RUN.sh --sim --help`

Example usage:
```
./RUN.sh --sim --b1 minmax --b1_args "-c configs/eval_params/BetterEval/example.json" --b2 mcts --b2_args "-l 42 -c configs/eval_params/BetterEval/example.json" --map map10x10.json --log_name test --games 2 --ready_timeout 1 --move_timeout 1 --game_timeout 10
```

### `rerun-simulation`, `--sim-r`
Used to rerun last simulation. The last simulation configuration is stored in [`configs/simulations/last`](configs/simulations/last) file which is updated in every run.

### `concurrent-rerun-simulation`, `-csim-r`
The concurrent version of `rerun-simulation`, used to rerun last simulation with each game run on separate thread.

### `run-simulation-from-config`, `--sim-c`
Used to run simulation with parameters given in configuration file. When you run `./RUN.sh --sim-c` you will be prompted to select one of available configurations stored in [`configs/simulations`](configs/simulations) directory.
You can also specify the configuration file name with `--config` option.

Example usage:
```
./RUN.sh --sim-c --config example.json
```

# <b> BOT ARENA </b>

<p align="center">
  <img src="ReadmeAssets/Logo.gif" alt="Image unavailable">
</p>

## Contents
- [How to use?](#how-to-use)
- [How does the program work?](#how-does-the-program-work)
- [Projects structure](#projects-structure)
- [Creating maps](#creating-maps)
- [Creating bots](#creating-bots)
- [What is log?](#what-is-log)


## How to use?

### Installation

1. Download [python interpreter](https://www.python.org/downloads/).
2. Download python modules
```
pip3 install pygame click numpy
```
3. `git clone https://github.com/MaksWisniewski/bot-arena/`
4. Download gcc13.
5. Intall cpp library `https://github.com/nlohmann/json`
**or Recomended**
*use docker via RUN.sh*

### Working versions
```
Python 3.12.2

Package    Version
---------- -------
click      8.1.7
numpy      1.26.4
pip        23.3.1
pygame     2.5.2
setuptools 68.2.2
wheel      0.41.2
```

### Activation
1. Go to the root folder of the project in terminal.
2. Start the app with "py main.py" instuction.
3. Or use bash script `RUN.sh`

### Usage
The program consists of 2 scenes where user can set up a simulation and a scene, where he can watch and analyze the bots' performance. Each scene consists of windows with different functionalities.

- ### Simulation creation scene
Allows user to configure and generate a simulation in which multiple games could be carried out. This scene is made up with 5 windows:
1. Bots - List of bots from which user can choose 2 bots that will play against one another in simulation that is being genereated.
2. Maps - List of maps the simulation can be generated on.
3. Add random map - Tool for creating new maps. After clicking "generate map" button a random map with given name will appear in the list of maps.
4. Control - Panel where user can see the names of bots and map that will be used in simulation and adjust:
    - Number of games - Amount of games that will be played in simulation.
    - Timeout fields for which simulations generation will be terminated if any of the specified timeout values are exceeded.
        - Max ready timeout - Time for bot to preprocess the game.
        - Max move timeout - Time for bot to make a move.
        - Max game timeout - Time for bots to play out entire game.
    - Simulation's name

    After specifing those fields user can push "start simulation" button that will start generating game's for that simulation.
5. Progress - Contains information about current progress of generation of the simulation and number of games each bot has won.

<p align="center">
  <img src="ReadmeAssets/Creation scene.png" alt="Image unavailable">
</p>

- ### Simulation launch scene
Gives user functionality of inspecting one log from previously generated simulations and running it. This scene consists of 4 windows:
1. Simulations - List of simulations previously generated by user.
2. Logs - List of logs that make up the selected simulation.
3. Simulation info - Gives user information about active simulation:
    - Simulation's name.
    - Which map the simulation was generated on.
    - Names of the bot's that played in this simulation.
    - Number of games each bot has won.
4. Log info - Gives user information about active log:
    - Log's name.
    - Name of the bot that has won the game.
    - Amount of turns the bots took to end the game.

<p align="center">
  <img src="ReadmeAssets/Launch scene.png" alt="Image unavailable">
</p>

- ### Simulation analyzing scene
Allows user to analyze the performance of each bot based on the log that has been chosen previously in [simulation launch scene](#simulation-launch-scene). This scene consists of 4 windows:
1. Log - List of turns from chosen log where the highlighted turn is the decision the bots made in current turn.
2. Game - Graphical representation of the state of game played in real time.
3. Stats - Resources of both bots.
4. Controls - Panel to control the general game flow:
    - Game speed - field where you can adjust how fast the simulation is being played out or even pause it.
    - Zoom - field where you can adjust how much the game's graphical representation is magnified.
    - Helpers:
        - eye icon - Option to see where tiles that are occupied by obstacles
        - pointer icon - Displays a little red dot indicating each objects real position.
        - grid icon - Shows a grid that marks where the tiles are.

<p align="center">
  <img src="ReadmeAssets/Analyzing scene.png" alt="Image unavailable">
</p>

## How does the program work?

## Projects structure
- [assets](/assets) Assets for game's graphical representation.
- [bots](/bots) Bot scripts.
- [logs](/maps) Maps the game is played on.
- [packages](/packages) Program's logic.
    - [game logic](/packages/game_logic) Module responisble for game's logic.
    - [gui](/packages/gui) Module responsible for the program's graphical representation.
        - [game renderer](/packages/gui/game_render) Game's graphical represention.
        - [gui_objects](/packages/gui/gui_objects/) User interface's graphical representation.
        - [scenes](/packages/gui/scenes) Views of the program.
    - [simulator](/packages/simulator) Module responsible for bot's logic.

## Creating maps
The maps that bots can play thier games on are represented as json files with 4 attributes:
- MAP_SIZE_X - value specifing the width of the map.
- MAP_SIZE_Y - value specifing the height of the map.
- path - list of ordered pairs (2-tuple) that specifies the path that the units will be walking on in the game.
- obstacles - list of ordered pairs (2-tuple) that specifies
placement of the objects that take up tiles to prevent placing buildings there

The path has to:
- start in point (0, 0)
- end in point (X_SIZE - 1, Y_SIZE - 1)
- have the next tile placed with it's side connected to the previous tile's in the path side.

<p align="center">
  <img src="ReadmeAssets/Map info.png" alt="Image unavailable" width="46%">
  <img src="ReadmeAssets/Map Showcase.png" alt="Image unavailable" width="50%">
</p>

## Creating bots
### Bot standard
#### Bot Package Python
To simplify the implementation of bots in Python for Bot Arena, a dedicated package is available. This package provides a class template, `Bot`, that offers tools for easier implementation of your own agents.
Explore the [Bot Package on GitHub](https://github.com/MaksWisniewski/bot-arena/tree/main/bot_package) for seamless bot implementation in Python for Bot Arena.


## What is log?
Log is our handy way of keeping track what choice did each bot in every turn. So to answer the question log is a text file that in each line has information about a turn in played game. There is a convention to the name and content of a log file.
### Naming convention
index_bot1_bot2_win.log where:
- index - index of log in the whole generated simulation
- bot1, bot2 - names of the bots that played in this game
- win - information about which bot won (0 - first bot won, 1 - second bot won)

For example: "4_random_bot.py_spawn_bot.py_1.log"

### Content convention
In the first 3 lines the log keeps map, player1, player2 values which are names of the players and map that that where used in this log's game.

The rest of the lines has a form as follows:
Action_of_first_player | Action_of_second_player

Player can make an action of 3 types:
- Place a building
    - F x y - builds farm at (x, y) coordinates.
    - T x y - builds tower at (x, y) coordinates.
- Spawn a soldier
    - S swordsman - spawns a swordsman that is a melee unit.
    - S archer - spawns an archer that is a ranged unit.
- Skip turn
    - W - don't do anything in this turn.

---

# Additional info

possible fix for conda enviroment, with radeon egpu
```bash
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6
```
