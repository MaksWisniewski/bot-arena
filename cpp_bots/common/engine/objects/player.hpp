#pragma once

#include "building.hpp"
#include "soldier.hpp"

#include <vector>

struct Player
{
    std::vector<Building> farms;
    std::vector<Building> turrets;
    std::vector<Soldier> soldiers;

    int gold;
    int income;
};
