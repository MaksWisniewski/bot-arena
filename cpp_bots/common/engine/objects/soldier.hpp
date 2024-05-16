#pragma once

#include "../../json.hpp"

struct Soldier
{
    enum class Type
    {
        swordsman,
        archer
    };

    Soldier() = default;
    Soldier(const Json&);
    Soldier(Type, int hp, int position);
    Soldier(const std::string& type, int hp, int position);

    Type type;

    int hp;
    int position;
    bool is_in_fight;
};

Soldier::Type string_to_soldier_type(const std::string&);
std::string soldier_type_to_string(Soldier::Type);
