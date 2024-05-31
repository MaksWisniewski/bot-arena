import time

from packages.game_logic.game import Game
from packages.simulator.api.sim_config import SimConfig
from packages.simulator.core.log_maker import LogMaker
from packages.simulator.core.runner.wait_players_ready import wait_players_ready
from packages.simulator.core.runner.play_game_till_timeout import play_game_till_timeout
from packages.simulator.core.runner.reset import reset


# Function to play a single game
def play_game(
        log_name,
        log_number,
        map_name,
        player_instances: dict,
        config: SimConfig):
    game = Game(map_name)
    log_maker = LogMaker(log_name, log_number)
    reset(player_instances, config, game)

    # Set timeout values
    ready_end_time = time.time() + config.ready_timeout
    game_end_time = time.time() + config.game_timeout
    is_ready = {player: False for player in player_instances.values()}

    ready_res = wait_players_ready(ready_end_time, is_ready, player_instances, log_maker, config)
    if ready_res:
        return ready_res
    return play_game_till_timeout(game_end_time, player_instances, log_maker, config, game)


def play_game_isolated(config, idx):
    player_instances = {}
    results = []
    LogMaker.clear(config.log_name)

    play_game(
        config.log_name,
        str(idx),
        config.map_name,
        player_instances,
        config)

    for player in player_instances.values():
        player.put('END')

    for player in player_instances.values():
        player.put('BYE')
        player.kill()
    return results
