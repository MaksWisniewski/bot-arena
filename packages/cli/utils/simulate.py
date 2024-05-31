from packages.simulator.api.sim_config import SimConfig
from packages.simulator.api.playSimulation import playSimulation, playSimulationConcurrent


def simulate(config: SimConfig):
    simulations = playSimulation(config)

    for i, simulation in enumerate(simulations):
        print(f'[ {i + 1} / {config.games} ] > {simulation}')
    print("After simulation")


def concurrentSimulate(config: SimConfig):
    simulations = playSimulationConcurrent(config)

    for i, simulation in enumerate(simulations):
        print(f'[ {i + 1} / {config.games} ] > {simulation}')

    print("After simulation")
