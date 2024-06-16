import json
import os
import pathlib

from packages.game_logic.economy.types.EconomyFarm import EconomyFarm
from packages.game_logic.economy.types.EconomySoldier import EconomySoldier
from packages.game_logic.economy.types.EconomyTurret import EconomyTurret
from packages.cli import ECONOMY_CONFIGS_DIR

class EconomyFlat:
    def __init__(self,
                 swordsmanStats: EconomySoldier,
                 archerStats: EconomySoldier,
                 farmStats: EconomyFarm,
                 turretStats: EconomyTurret,
                 passive_gold: int,
                 start_gold: int):
        self.soldiers = {
            "swordsman": swordsmanStats.__dict__,
            "archer": archerStats.__dict__
        }
        self.buildings = {
            "farm": farmStats.__dict__,
            "turret": turretStats.__dict__
        }
        self.passive_gold = passive_gold
        self.start_gold = start_gold

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

    @classmethod
    def fromFilename(cls, name):
        filepath = os.path.join(ECONOMY_CONFIGS_DIR, name)
        cls.fromFile(filepath)


