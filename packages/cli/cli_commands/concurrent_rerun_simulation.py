import click
from packages.cli import *
from packages.cli.utils.simulate import concurrentSimulate
from packages.simulator.api.sim_config import SimConfig


@click.command()
def concurrent_rerun_simulation():
    config = SimConfig.fromFile(SIMFILE_LAST)
    concurrentSimulate(config)
