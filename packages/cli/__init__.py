import sys, os
from pathlib import Path
import packages

CONFIGS_DIR = str(Path(__file__).parent.parent.parent) + "/configs/"
SIM_CONFIGS_DIR = CONFIGS_DIR + "simulations"
SIMFILE_LAST = SIM_CONFIGS_DIR + "last"

BOTS = os.listdir(packages.BOTS_DIRECTORY)
MAPS = os.listdir(packages.MAPS_DIRECTORY)
SIM_CONFIGS = os.listdir(SIM_CONFIGS_DIR)

SEPARATOR = "\n\t> "

BOTS_STR = SEPARATOR + SEPARATOR.join(BOTS)
MAPS_STR = SEPARATOR + SEPARATOR.join(MAPS)
SIM_CONFIGS_STR = SEPARATOR + SEPARATOR.join(SIM_CONFIGS)