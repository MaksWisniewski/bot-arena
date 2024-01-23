# <b> BOT ARENA </b>

<p align="center">
  <img src="ReadmeAssets/Logo.gif" alt="Image unavailable">
</p>

## Contents
- [Few words about project](#few-words-about-project)
- [How to use?](#how-to-use)
- [How does the program work?](#how-does-the-program-work)
- [Projects structure](#projects-structure)
- [Creating maps](#creating-maps)
- [Creating bots](#creating-bots)
- [What is log? (baby don't hurt me)](#what-is-log)

## Few words about project
"Bot arena" is a group project implemented under the "Innovative Projects by Nokia", which main goal was to create an environment where users can watch and test performance and efficiency of bots written by them.

## How to use?

### Installation
1. Download [python interpreter](https://www.python.org/downloads/).
2. Download external pygame module with "pip install pygame".
3. Download the [repository](https://github.com/BartoszKruszewski/bot-arena.git) or clone it with "git clone https://github.com/BartoszKruszewski/bot-arena.git".

### Activation
1. Go to the root folder of the project in terminal.
2. Start the app with "py main.py" instuction.

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

## Creating bots

## What is log?
<!-- Typy pól:
- spawn jednostek dla przeciwnika (S)
- moja wieza (T)
- farma waluty (F) -->
