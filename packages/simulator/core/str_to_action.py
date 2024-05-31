from packages.game_logic.actions import Wait, BuildTurret, BuildFarm, SpawnSoldier
from packages.simulator.api.WrongMove import WrongMove


# Function to convert a string into a corresponding action object
def str_to_action(str, player):
    side = "left" if player == 0 else "right"
    str = str.split()
    if str[0] == "W":
        return Wait(side)
    elif str[0] == "T":
        return BuildTurret(side, str[1], str[2])
    elif str[0] == "F":
        return BuildFarm(side, str[1], str[2])
    elif str[0] == "S":
        return SpawnSoldier(side, str[1])
    else:
        raise WrongMove
