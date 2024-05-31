from packages.simulator.api.sim_config import SimConfig
from packages.simulator.core.runner.play_game import play_game, play_game_isolated
from packages.simulator.core.log_maker import LogMaker
from concurrent.futures import ThreadPoolExecutor


# Main function for playing multiple games
def play(config: SimConfig):
    player_instances = {}
    results = []
    LogMaker.clear(config.log_name)

    for i in range(config.games):
        yield play_game(
            config.log_name,
            str(i),
            config.map_name,
            player_instances,
            config)
        for player in player_instances.values():
            player.put('END')

    # End the game and kill player processes
    for player in player_instances.values():
        player.put('BYE')
        player.kill()
    return results


# Plays each simulation on separate thread
def playConcurrent(config: SimConfig):

    with ThreadPoolExecutor(max_workers=config.games) as executor:
        tasks = [executor.submit(play_game_isolated, config, i) for i in range(config.games)]
        results = [task.result() for task in tasks]

    return results
