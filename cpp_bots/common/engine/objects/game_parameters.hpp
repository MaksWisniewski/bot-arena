#pragma once

#include "soldier.hpp"

#include <unordered_map>

struct GameParameters
{
    struct SoldierParameters
    {
        int max_hp;
        int damage;
        int range;
        int cost;
    };

    std::unordered_map<Soldier::Type, SoldierParameters> soldiers;

    struct FarmParameters
    {
        int gold;
        int cost;
    } farm;

    struct TurretParameters
    {
        int attack;
        int range;
        int cost;
    } turret;
};
