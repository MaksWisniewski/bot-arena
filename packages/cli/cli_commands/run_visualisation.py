import click
from packages.cli import *

import packages
import packages.gui
import packages.gui.gui_objects
import packages.gui.scenes
import packages.gui.scenes.game_scene as game_scene


@click.command()
def run_visualisation():
    print("Visualisation should be displayed.")
    scene_manager = game_scene.GameSceneManager()
    # scene_manager.load_scene({
    #     'log_name': 'temp/0_mcts_minmax_TIE.log'
    # })
