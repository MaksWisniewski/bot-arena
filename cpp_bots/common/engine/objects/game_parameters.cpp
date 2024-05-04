#include "game_parameters.hpp"

GameParameters::GameParameters(const Json& json) :
    soldiers{
        {Soldier::Type::swordsman, json["soldiers"]["swordsman"]},
        {Soldier::Type::archer, json["soldiers"]["archer"]}
    },
    farm{json["buildings"]["farm"]},
    turret{json["buildings"]["turret"]}
{
}

GameParameters::SoldierParameters::SoldierParameters(const Json& json) :
    max_hp{json["max_hp"]},
    damage{json["damage"]},
    range{json["range"]},
    cost{json["cost"]}
{
}

GameParameters::FarmParameters::FarmParameters(const Json& json) :
    gold{json["gold"]},
    cost{json["cost"]}
{
}

GameParameters::TurretParameters::TurretParameters(const Json& json) :
    attack{json["attack"]},
    range{json["range"]},
    cost{json["cost"]}
{
}
