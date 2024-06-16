import json


class SimConfig:
    def __init__(self,
                 bot_left,
                 bot_left_args,
                 bot_right,
                 bot_right_args,
                 map_name,
                 log_name,
                 games,
                 ready_timeout,
                 move_timeout,
                 game_timeout,
                 economy_name: str):
        self.bot_left = bot_left
        self.bot_left_args = bot_left_args
        self.bot_right = bot_right
        self.bot_right_args = bot_right_args
        self.map_name = map_name
        self.log_name = log_name
        self.games = games
        self.ready_timeout = ready_timeout
        self.move_timeout = move_timeout
        self.game_timeout = game_timeout
        self.economy_name = economy_name

    def toList(self):
        return list(self.__dict__.values())

    def serialize(self):
        return json.dumps(
            self,
            default=lambda o: o.__dict__,
            sort_keys=True,
            indent=4)

    def toFile(self, path):
        with open(path, 'w') as outfile:
            outfile.write(self.serialize())

    @classmethod
    def deserialize(cls, serialized):
        return cls(**json.loads(serialized))

    @classmethod
    def fromFile(cls, path):
        with open(path, "r") as file:
            return cls.deserialize(file.read())
