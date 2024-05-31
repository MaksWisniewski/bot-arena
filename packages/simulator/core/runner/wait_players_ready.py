import sys
import time

from packages.simulator.core.runner.determine import determine


def wait_players_ready(
        ready_end_time,
        is_ready,
        player_instances,
        log_maker,
        config):
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
        log_maker.save("TIE", config.map_name, config.bot_left, config.bot_right)
        return determine(is_ready, player_instances)
