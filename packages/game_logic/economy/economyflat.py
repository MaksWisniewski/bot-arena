import json
import os
import pathlib

from packages.game_logic.economy.EconomyFarm import EconomyFarm
from packages.game_logic.economy.EconomySoldier import EconomySoldier
from packages.game_logic.economy.EconomyTurret import EconomyTurret


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
        configsPath = pathlib.Path(__file__).parent.parent.resolve()
        filepath = os.path.join(configsPath, name)
        cls.fromFile(filepath)

class EconomySoldiers:
    swordsman: EconomySoldier
    archer: EconomySoldier
class Economy:
    def __init__(self, economy: EconomyFlat):
        self._updateSoldiers(economy)
    def _updateSoldiers(self, economy: EconomyFlat):
        soldiers = EconomySoldiers
        soldiers.swordsman = EconomySoldier.__dict__ = economy.soldiers["swordsman"]
        soldiers.archer = EconomySoldier.__dict__ = economy.soldiers["archer"]
        self.soldiers = soldiers
