import sys, os
from pathlib import Path
# sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import click
import packages

from packages.gui.main import Main
from packages.simulator.sim_config import SimConfig
from packages.cli.simulate import simulate

CONFIGS_DIR = str(Path(__file__).parent.parent.parent) + "/configs/"
SIM_CONFIGS_DIR = CONFIGS_DIR + "simulations"
SIMFILE_LAST = SIM_CONFIGS_DIR + "last"

BOTS = os.listdir(packages.BOTS_DIRECTORY)
MAPS = os.listdir(packages.MAPS_DIRECTORY)
SIM_CONFIGS = os.listdir(SIM_CONFIGS_DIR)

SEPARATOR = "\n\t> "

BOTS_STR = SEPARATOR + SEPARATOR.join(BOTS)
MAPS_STR = SEPARATOR + SEPARATOR.join(MAPS)
SIM_CONFIGS_STR = SEPARATOR + SEPARATOR.join(SIM_CONFIGS)


@click.command()
@click.option("--b1",
              prompt=f'Select one of bots:{BOTS_STR}\n',
              help='Left bot for simulation')
@click.option("--b2",
              prompt=f'Select one of bots:{BOTS_STR}\n',
              help='Right bot for simulation')
@click.option("--map",
              prompt=f'Select one of maps: {MAPS_STR}\n',
              help='Map for simulation')
@click.option("--log_name", default="temp", help='Name of log file')
@click.option("--games", default=1, help='Number of games to simulate')
@click.option("--ready_timeout", default=10)
@click.option("--move_timeout", default=10)
@click.option("--game_timeout", default=600)
def run_simulation(
        b1,
        b2,
        map,
        log_name,
        games,
        ready_timeout,
        move_timeout,
        game_timeout):
    config = SimConfig(
        bot_left=b1,
        bot_right=b2,
        map_name=map,
        log_name=log_name,
        games=games,
        ready_timeout=ready_timeout,
        move_timeout=move_timeout,
        game_timeout=game_timeout)

    config.toFile(SIMFILE_LAST)
    simulate(config)


@click.command()
def rerun_simulation():
    config = SimConfig.fromFile(SIMFILE_LAST)
    simulate(config)


@click.command()
@click.option("--sim_config",
              prompt=f'Select one of configurations:{SIM_CONFIGS_STR}\n',
              help='Simulation config file')
def run_simulation_from_config(sim_config):
    try:
        config = SimConfig.fromFile(sim_config)
    except:
        raise LookupError(f"Failed to load config file {sim_config}")

    simulate(config)


@click.command()
def run_gui():
    Main()


@click.group
def cli(): pass


cli.add_command(run_simulation)
cli.add_command(rerun_simulation)
cli.add_command(run_simulation_from_config)
cli.add_command(run_gui)

if __name__ == "__main__":
    cli()
