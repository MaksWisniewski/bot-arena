from packages.simulator.core.play import play, playConcurrent
from packages.simulator.api.sim_config import SimConfig


def playSimulation(config: SimConfig):
    return play(config)


def playSimulationConcurrent(config: SimConfig):
    return playConcurrent(config)
