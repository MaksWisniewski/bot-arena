import sys, os
from pathlib import Path
import packages

ROOTPATH = str(Path(__file__).parent.parent.parent)
CONFIGS_DIR = ROOTPATH + "/configs/"
SIM_CONFIGS_DIR = CONFIGS_DIR + "simulations"
SIMFILE_LAST = SIM_CONFIGS_DIR + "/last"
ECONOMY_CONFIGS_DIR = CONFIGS_DIR + "economies"
ECONOMY_ACTIVE_PATH = ROOTPATH + "/packages/game_logic/stats.py"

BOTS = os.listdir(packages.BOTS_DIRECTORY)
MAPS = os.listdir(packages.MAPS_DIRECTORY)
SIM_CONFIGS = os.listdir(SIM_CONFIGS_DIR)
ECONOMIES = os.listdir(ECONOMY_CONFIGS_DIR)

SEPARATOR = "\n\t> "

BOTS_STR = SEPARATOR + SEPARATOR.join(BOTS)
MAPS_STR = SEPARATOR + SEPARATOR.join(MAPS)
SIM_CONFIGS_STR = SEPARATOR + SEPARATOR.join(SIM_CONFIGS)
ECONOMIES_STR = SEPARATOR + SEPARATOR.join(ECONOMIES)