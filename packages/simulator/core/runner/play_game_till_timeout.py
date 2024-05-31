import sys
import time

from packages.game_logic.game import ErrorCode
from packages.simulator.api.WrongMove import WrongMove
from packages.simulator.core.runner.determine import determine
from packages.simulator.core.serializer import Serializer
from packages.simulator.core.str_to_action import str_to_action


def play_game_till_timeout(game_end_time,
                           player_instances,
                           log_maker,
                           config,
                           game):
    while time.time() < game_end_time:
        action = {player: None for player in player_instances.values()}
        move_end_time = min(time.time() + config.move_timeout, game_end_time)
        while time.time() < move_end_time:
            for player in player_instances.values():
                if not action[player]:
                    action[player] = player.get()
            if all(action.values()):
                break
        else:
            print(__name__, f"end by {'game' if move_end_time == game_end_time else 'move'} timeout",
                  file=sys.stderr)
            log_maker.save("TIE", config.map_name, config.bot_left, config.bot_right)
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
        response = game.update(action[player_instances[0]], action[player_instances[1]])

        # Check for game end conditions and save the log
        if response[0] == ErrorCode[1]:
            log_maker.save("0", config.map_name, config.bot_left, config.bot_right)
            return 0
        elif response[0] == ErrorCode[2]:
            log_maker.save("1", config.map_name, config.bot_left, config.bot_right)
            return 1
        elif "Tie" in response:
            log_maker.save("TIE", config.map_name, config.bot_left, config.bot_right)
            print(__name__, "end by tie", file=sys.stderr)
            return 'TIE'

        # Send game data to players
        game_data = Serializer.get_json(game)
        actions = f"{action[player_instances[0]]} | {action[player_instances[1]]}"
        for player in player_instances.values():
            player.put(actions)
            player.put(game_data)
    else:
        print(__name__, f"end by game timeout", file=sys.stderr)
        log_maker.save("TIE", config.map_name, config.bot_left, config.bot_right)

    return "TIE"
