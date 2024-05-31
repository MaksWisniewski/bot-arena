from packages.simulator.api.sim_config import SimConfig
from packages.simulator.api.playSimulation import playSimulation


def simulate(config: SimConfig):
    simulations = playSimulation(config)

    for i, simulation in enumerate(simulations):
        print(f'[ {i + 1} / {config.games} ] > {simulation}')
    print("After simulation")
