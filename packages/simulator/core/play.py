import sys
import time

from packages.simulator.api.sim_config import SimConfig
from packages.game_logic.game import Game, ErrorCode
from packages.simulator.api.WrongMove import WrongMove
from packages.simulator.core.runner.reset import reset
from packages.simulator.core.str_to_action import str_to_action
from packages.simulator.core.log_maker import LogMaker
from packages.simulator.core.serializer import Serializer


# Function to determine the winner of a game based on the player's state
def determine(players_state: dict[int, bool], player_instances: dict):
    if not players_state[player_instances[0]] and not players_state[player_instances[1]]:
        return 'TIE'
    elif not players_state[player_instances[0]]:
        return 1
    elif not players_state[player_instances[1]]:
        return 0


# Function to play a single game
def play_game(log_name, log_number, map_name):
    # Initialize the game and log maker
    gameX = Game(map_name)
    log_maker = LogMaker(log_name, log_number)
    reset(player_instances, config, game)

    # Set timeout values
    ready_end_time = time.time() + config.ready_timeout
    game_end_time = time.time() + config.game_timeout
    is_ready = {player: False for player in player_instances.values()}

    # Wait for players to be ready
    while time.time() < ready_end_time:
        for player in player_instances.values():
            if not is_ready[player]:
                response = player.get()
                if response == 'READY':
                    is_ready[player] = True
        if all(is_ready.values()):
            break
    else:
        print(__name__, "end by ready timeout", file=sys.stderr)
        log_maker.save("TIE", map_name, config.bot_left, config.bot_right)
        return determine(is_ready, player_instances)

    # Play the game until timeout
    while time.time() < game_end_time:
        action = {player: None for player in player_instances.values()}
        move_end_time = min(time.time() + move_timeout, game_end_time)
        while time.time() < move_end_time:
            for player in player_instances.values():
                if not action[player]:
                    action[player] = player.get()
            if all(action.values()):
                break
        else:
            print(__name__, f"end by {'game' if move_end_time == game_end_time else 'move'} timeout",
                  file=sys.stderr)
            log_maker.save("TIE", map_name, bot_left, bot_right)
            return determine(action, player_instances)

        # Add actions to the log
        log_maker.add_actions(*action.values())

        try:
            action[player_instances[0]] = str_to_action(action[player_instances[0]], 0)
        except WrongMove:
            print(__name__, "end by wrong move", file=sys.stderr)
            return 1
        try:
            action[player_instances[1]] = str_to_action(action[player_instances[1]], 1)
        except WrongMove:
            print(__name__, "end by wrong move", file=sys.stderr)
            return 0

        # Update the game state and get the response
        response = gameX.update(action[player_instances[0]], action[player_instances[1]])

        # Check for game end conditions and save the log
        if response[0] == ErrorCode[1]:
            log_maker.save("0", map_name, bot_left, bot_right)
            return 0
        elif response[0] == ErrorCode[2]:
            log_maker.save("1", map_name, bot_left, bot_right)
            return 1
        elif "Tie" in response:
            log_maker.save("TIE", map_name, bot_left, bot_right)
            print(__name__, "end by tie", file=sys.stderr)
            return 'TIE'

        # Send game data to players
        game_data = Serializer.get_json(gameX)
        actions = f"{action[player_instances[0]]} | {action[player_instances[1]]}"
        for player in player_instances.values():
            player.put(actions)
            player.put(game_data)
    else:
        print(__name__, f"end by game timeout", file=sys.stderr)
        log_maker.save("TIE", map_name, bot_left, bot_right)

    return "TIE"


# Main function for playing multiple games
def play(config: SimConfig):
    player_instances = {}
    game = Game(config.map_name)
    results = []

    # Play the specified number of games
    LogMaker.clear(config.log_name)
    for i in range(config.games):
        yield play_game(config.log_name, str(i), config.map_name)
        for player in player_instances.values():
            player.put('END')

    # End the game and kill player processes
    for player in player_instances.values():
        player.put('BYE')
        player.kill()
    return results
