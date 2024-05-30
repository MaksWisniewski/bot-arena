# Simulator documentation

## Code Organization

### `sim_config.py`

Simulation config class to cleanup mess with passing gazillion of arguments to simulation.

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