import json


class SimConfig:
    def __init__(self,
                 bot_left,
                 bot_right,
                 map_name,
                 log_name,
                 games,
                 ready_timeout,
                 move_timeout,
                 game_timeout):
        self.bot_left = bot_left
        self.bot_right = bot_right
        self.map_name = map_name
        self.log_name = log_name
        self.games = games
        self.ready_timeout = ready_timeout
        self.move_timeout = move_timeout
        self.game_timeout = game_timeout

    def serialize(self):
        return json.dumps(
            self,
            default=lambda o: o.__dict__,
            sort_keys=True,
            indent=4)

    def toList(self):
        return list(self.__dict__.values())

    @classmethod
    def deserialize(cls, serialized):
        return cls(**json.loads(serialized))
