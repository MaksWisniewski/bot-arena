import click
from packages.cli import *
from packages.cli.utils.simulate import simulate
from packages.simulator.api.sim_config import SimConfig


@click.command()
@click.option("--b1",
              prompt=f'Select one of bots:{BOTS_STR}\n',
              help='Left bot for simulation')
@click.option("--b1_args", default="", help='Command line arguments for left bot')
@click.option("--b2",
              prompt=f'Select one of bots:{BOTS_STR}\n',
              help='Right bot for simulation')
@click.option("--b2_args", default="", help='Command line arguments for right bot')
@click.option("--map",
              prompt=f'Select one of maps: {MAPS_STR}\n',
              help='Map for simulation')
@click.option("--log_name", default="temp", help='Name of log file')
@click.option("--games", default=1, help='Number of games to simulate')
@click.option("--ready_timeout", default=5)
@click.option("--move_timeout", default=2)
@click.option("--game_timeout", default=60)
def run_simulation(
        b1,
        b1_args,
        b2,
        b2_args,
        map,
        log_name,
        games,
        ready_timeout,
        move_timeout,
        game_timeout):
    config = SimConfig(
        bot_left=b1,
        bot_left_args=b1_args,
        bot_right=b2,
        bot_right_args=b2_args,
        map_name=map,
        log_name=log_name,
        games=games,
        ready_timeout=ready_timeout,
        move_timeout=move_timeout,
        game_timeout=game_timeout)

    config.toFile(SIMFILE_LAST)
    simulate(config)
