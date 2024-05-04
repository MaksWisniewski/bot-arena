#pragma once

#include "../../json.hpp"
#include "building.hpp"
#include "soldier.hpp"

#include <vector>

struct Player
{
    // Player(const Json&);

    std::vector<Building> farms;
    std::vector<Building> turrets;
    std::vector<Soldier> soldiers;

    int gold;
    int income;
};
