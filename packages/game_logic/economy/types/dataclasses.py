from dataclasses import dataclass
from packages.game_logic.economy.types.EconomyFarm import EconomyFarm
from packages.game_logic.economy.types.EconomySoldier import EconomySoldier
from packages.game_logic.economy.types.EconomyTurret import EconomyTurret


@dataclass
class EconomySoldiers:
    swordsman: EconomySoldier
    archer: EconomySoldier


@dataclass
class EconomyBuildings:
    farm: EconomyFarm
    turret: EconomyTurret
