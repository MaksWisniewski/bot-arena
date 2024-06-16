import click
import shutil
from packages.cli import *


@click.command()
@click.option("--economy",
              prompt=f'Select one of bots:{ECONOMIES_STR}\n',
              help='Select economy')
def select_economy(economy):
    selected_economy = f'{ECONOMY_CONFIGS_DIR}/{economy}'
    shutil.copy(selected_economy, ECONOMY_ACTIVE_PATH)
