#pragma once

struct Soldier
{
    enum class Type
    {
        swordsman,
        archer
    } type;

    int hp;
    int position;
};
