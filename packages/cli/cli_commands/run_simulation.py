import click
from packages.cli import *
from packages.cli.simulate import simulate
from packages.simulator.sim_config import SimConfig


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
