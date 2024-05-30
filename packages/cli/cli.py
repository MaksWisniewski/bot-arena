import sys, os
from pathlib import Path
# sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import click
import packages

from packages.simulator.api import playSimulation
from packages.gui.main import Main
from packages.simulator.sim_config import SimConfig


BOTS = os.listdir(packages.BOTS_DIRECTORY)
MAPS = os.listdir(packages.MAPS_DIRECTORY)
SEPARATOR = "\n\t> "
BOTS_STR = SEPARATOR + SEPARATOR.join(BOTS)
MAPS_STR = SEPARATOR + SEPARATOR.join(MAPS)

CONFIGS_DIR = str(Path(__file__).parent.parent.parent) + "/configs/"
SIMFILE_LAST = CONFIGS_DIR + "simulations/last.json"

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
        game_timeout=move_timeout)

    simulations = playSimulation(config)
    config.toFile(SIMFILE_LAST)

    for i, simulation in enumerate(simulations):
        print(f'[ {i + 1} / {games} ] > {simulation}')
    print("After simulation")


@click.command()
def run_gui():
    Main()


@click.group
def cli(): pass


cli.add_command(run_simulation)
cli.add_command(run_gui)

if __name__ == "__main__":
    cli()
