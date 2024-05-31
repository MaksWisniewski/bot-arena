from packages.simulator.api.sim_config import SimConfig
from packages.simulator.core.runner.play_game import play_game
from packages.simulator.core.log_maker import LogMaker


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

