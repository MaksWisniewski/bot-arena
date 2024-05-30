import click
from packages.cli import *
from packages.cli.utils.simulate import simulate
from packages.simulator.sim_config import SimConfig


@click.command()
def rerun_simulation():
    config = SimConfig.fromFile(SIMFILE_LAST)
    simulate(config)
