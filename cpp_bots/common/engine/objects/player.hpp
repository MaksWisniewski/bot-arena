#pragma once

#include "../../json.hpp"
#include "building.hpp"
#include "soldier.hpp"

#include <vector>

struct Player
{
    Player() = default;
    Player(const Json&);

    // TODO: maybe consider using std::list for soldiers ???
    std::vector<Building> farms;
    std::vector<Building> turrets;
    std::vector<Soldier> soldiers;

    int gold;
    int income;
};
