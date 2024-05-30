# CLI Documentation

## Code Organization

### `__init__.py` 

Contains constants used in **CLI**.

### `cli.py`

Entry point of command line interface.

 ❗  CLI tested ONLY while being run from `RUN.sh` from project root.  |
|-----------------------------------------|


### `cli_commands/`

Click library uses a huge amount of decorators in function options declaration hence all commands are located in individual files.

### `cli_commands/run_simulation.py`

Runs a simulation and saves config to temporary file _file name in constant_ `SIMFILE_LAST`.

### `cli_commands/rerun_simulation.py`

Runs simulation from config file `SIMFILE_LAST`.

### `cli_commands/run_simulation_from_config.py`

Allows user to run simulation from any config file stored in [`configs/simulations`](../config/simulations)
