from packages.game_logic.game import Game
from packages.simulator.api.bot import Bot
from packages.simulator.api.sim_config import SimConfig
from packages.simulator.core.serializer import Serializer


# Function to reset the game state and initialize players if not already done
def reset(
        player_instances: dict,
        config: SimConfig,
        game: Game):
    kill_player_instances(player_instances, config)
    initialize_players(player_instances, config, game)


def kill_player_instances(player_instances: dict, config: SimConfig):
    if not player_instances:
        for playerX in player_instances.values():
            playerX.kill()
        player_instances[0] = Bot(config.bot_left)
        player_instances[1] = Bot(config.bot_right)


def initialize_players(player_instances: dict, config: SimConfig, game: Game):
    for i, player in enumerate(player_instances.values()):
        # Send settings to bots
        player.put(bot_settings_msg(config, i))

        game_data = Serializer.get_json(game)
        player.put(game_data)
        player.total_time = 0.0


def bot_settings_msg(config: SimConfig, i):
    side = 'left' if i == 0 else 'right'
    return "{} {} {} {}".format(
        config.game_timeout,
        config.move_timeout,
        config.ready_timeout,
        side
    )
