from packages.simulator.core.play import play
from packages.simulator.api.sim_config import SimConfig


def playSimulation(config: SimConfig):
    return play(config)
