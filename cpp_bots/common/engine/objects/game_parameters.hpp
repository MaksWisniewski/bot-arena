#pragma once

#include "../../json.hpp"
#include "soldier.hpp"

#include <unordered_map>

struct GameParameters
{
    GameParameters(const Json&);

    struct SoldierParameters
    {
        SoldierParameters() = default;
        SoldierParameters(const Json&);

        int max_hp;
        int damage;
        int range;
        int cost;
    };

    std::unordered_map<Soldier::Type, SoldierParameters> soldiers;

    struct FarmParameters
    {
        FarmParameters(const Json&);

        int gold;
        int cost;
    } farm;

    struct TurretParameters
    {
        TurretParameters(const Json&);

        int attack;
        int range;
        int cost;
    } turret;
};
