import click

import packages
from packages.gui.main import Main
from packages.cli.cli_commands.run_simulation import run_simulation
from packages.cli.cli_commands.rerun_simulation import rerun_simulation
from packages.cli.cli_commands.run_simulation_from_config import run_simulation_from_config
from packages.cli.cli_commands.run_visualisation import run_visualisation
from packages.cli.cli_commands.concurrent_rerun_simulation import concurrent_rerun_simulation
from packages.cli.cli_commands.select_economy import select_economy
@click.command()
def run_gui():
    Main()

@click.group
def cli(): pass


cli.add_command(run_simulation)
cli.add_command(rerun_simulation)
cli.add_command(concurrent_rerun_simulation)
cli.add_command(run_simulation_from_config)
cli.add_command(run_gui)
cli.add_command(run_visualisation)
cli.add_command(select_economy)

if __name__ == "__main__":
    cli()
