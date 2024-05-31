import click
from packages.cli import *
from packages.cli.utils.simulate import simulate
from packages.simulator.api.sim_config import SimConfig


@click.command()
@click.option("--config",
              prompt=f'Select one of configurations:{SIM_CONFIGS_STR}\n',
              help='Simulation config file')
def run_simulation_from_config(config):
    try:
        red_config = SimConfig.fromFile(f'{SIM_CONFIGS_DIR}/{config}')
    except:
        raise LookupError(f"Failed to load config file {config}")

    simulate(red_config)
