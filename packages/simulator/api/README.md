# Simulator API documentation


## Code organization

### `sim_config.py`

Simulation config class to clean up mess with passing gazillion of arguments to simulation.

----------------------
**Class fields**

- `bot_left` \ `bot_right` name of bot executable file / python file.
- `map_name`, `log_name` - as name suggests full name of file containing map or logfile.
- `games` - count of games to be run during simulation
-  **TIMEOUTS** \
   Simulation is divided into **pre-game** and **game** phase.
    - `ready_timeout`_(seconds)_ is time of **pre-game** phase for each bot. Time for preparation and preprocessing.
    - `game_timeout` _(seconds)_ is upper limit of **game** phase duration
    - `move_timeout` _(seconds)_ is time each player has to make a move during **game** phase.

### `playSimulation.py`

Contains only the function to run simulations.

`playSimulation(config)` where config is of type `SimConfig` described above.

### `wrongMove.py`

Contains `WrongMove` exception which is triggered once player tries to do illegal move.

### `bot.py`

Contains `Bot` class that creates bot processes and controls their communication with the arena.
