import sys, os

# sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import click
import packages
from packages.simulator.api import play
from packages.gui.main import Main

BOTS = os.listdir(packages.BOTS_DIRECTORY)
MAPS = os.listdir(packages.MAPS_DIRECTORY)
SEPARATOR = "\n\t> "
BOTS_STR = SEPARATOR + SEPARATOR.join(BOTS)
MAPS_STR = SEPARATOR + SEPARATOR.join(MAPS)

@click.command()
@click.option("--b1",
              prompt=f'Select one of bots:{ BOTS_STR }',
              help='Left bot for simulation')
@click.option("--b2",
              prompt=f'Select one of bots:{ BOTS_STR }',
              help='Right bot for simulation')
@click.option("--map",
              prompt=f'Select one of maps: { MAPS_STR }',
              help='Map for simulation')
@click.option("--log_name", default="temp", help='Name of log file')
@click.option("--games", default=1, help='Number of games to simulate')
@click.option("--ready_timeout", default=10)
@click.option("--move_timeout", default=10)
@click.option("--game_timeout", default=60)
def run_simulation(
        b1,
        b2,
        map,
        log_name,
        games,
        ready_timeout,
        move_timeout,
        game_timeout):
    print("Simulating")
    simulations = play(
        b1, b2, games, map, log_name,
        ready_timeout, move_timeout, game_timeout
    )
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
