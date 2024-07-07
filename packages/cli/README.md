# CLI Documentation

## Code Organization

### `__init__.py`

Contains constants used in **CLI**.

### `cli.py`

Entry point of command line interface.

| ❗CLI tested ONLY while being run from `RUN.sh` from project root.|
|-------------------------------------------------------------------|


### `cli_commands/`

Click library uses a huge amount of decorators in function options declaration hence all commands are located in individual files.

### `cli_commands/run_simulation.py`

Runs a simulation and saves config to temporary file _file name in constant_ `SIMFILE_LAST`.
Simulation is run using [`SimConfig`](/packages/simulator/api/README.md#`sim_config.py`) object.

### `cli_commands/rerun_simulation.py`

Runs simulation from config file `SIMFILE_LAST`.

### `cli_commands/concurrent_rerun_simulation.py`

Concurrent veersion of `rerun_simulation`.

### `cli_commands/run_simulation_from_config.py`

Allows user to run simulation from any config file stored in [`configs/simulations`](/configs/simulations)

### `cli_commands/select_economy.py`

Allows user to select economy configuration that will be used in simulation.
Currently configuration file is copied from [`configs/economies`](/configs/economies) to [`packages/game_logic/stats.py`](/packages/game_logic/stats.py) which is imported by game engine.

#### Economy configuration format (see [`packages/game_logic/stats.py`](/packages/game_logic/stats.py))

The economy configuration is a Python file with defined following constants:
- `COST: dict[str, int]` - defines cost of farm and turret, must contain keys `'turret'` and `'farm'`.
- `START_GOLD: int` - initial amount of gold that players start with.
- `PASSIVE_GOLD: int` - base income (without any farms).
- `FARM_GOLD: int` - gold produced by single farm in one turn.
- `TURRET_STATS: dict[str, int]` - turret parameters, contains keys `'attack'` and `'range'`.
- `SOLDIERS_STATS: dict[str, dict[str, int]]` - soldier parameters, at first level contains keys `'swordsman'` and `'archer'`, mapping soldier type to its parameters.
Each of them maps to a dictionary containing keys: `'max_hp'`, `'damage'`, `'range'`, `'cost'`.
- `OBSTACLES_AMOUNT: int` - number of obstacles on the map.
- `MAP_SIZE: tuple[int, int]` - map size, should be equal to `(MAP_SIZE_X, MAP_SIZE_y)`.
- `MAP_SIZE_X: int` - map width.
- `MAP_SIZE_Y: int` - map height.
