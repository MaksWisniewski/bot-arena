import click
from packages.cli import *
from packages.cli.simulate import simulate
from packages.simulator.sim_config import SimConfig


@click.command()
@click.option("--config",
              prompt=f'Select one of configurations:{SIM_CONFIGS_STR}\n',
              help='Simulation config file')
def run_simulation_from_config(sim_config):
    try:
        config = SimConfig.fromFile(f'{SIM_CONFIGS_DIR}/{sim_config}')
    except:
        raise LookupError(f"Failed to load config file {sim_config}")

    simulate(config)
