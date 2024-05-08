#pragma once

#include "../../json.hpp"

struct Soldier
{
    Soldier() = default;
    Soldier(const Json&);

    enum class Type
    {
        swordsman,
        archer
    } type;

    int hp;
    int position;
    bool is_in_fight;
};
