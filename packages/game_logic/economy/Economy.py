from packages.game_logic.economy.types.EconomyFarm import EconomyFarm
from packages.game_logic.economy.types.EconomySoldier import EconomySoldier
from packages.game_logic.economy.types.EconomyTurret import EconomyTurret
from packages.game_logic.economy.economyFlat import EconomyFlat
from packages.game_logic.economy.types.dataclasses import EconomySoldiers, EconomyBuildings


class Economy:
    def __init__(self, economy: EconomyFlat):
        self._updateSoldiers(economy)
        self._updateBuildings(economy)
        self.passive_gold = economy.passive_gold
        self.start_gold = economy.start_gold

    def _updateSoldiers(self, economy: EconomyFlat):
        self.soldiers = EconomySoldiers(
            swordsman=EconomySoldier(**economy.soldiers["swordsman"]),
            archer=EconomySoldier(**economy.soldiers["archer"])
        )

    def _updateBuildings(self, economy: EconomyFlat):
        self.buildings = EconomyBuildings(
            farm=EconomyFarm(**economy.buildings["farm"]),
            turret=EconomyTurret(**economy.buildings["turret"])
        )
