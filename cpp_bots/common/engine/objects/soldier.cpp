#include "soldier.hpp"

#include <string>

Soldier::Soldier(const Json& json) :
    type{string_to_soldier_type(json["type"])},
    hp{json["hp"]},
    position{json["position"]},
    is_in_fight{false}
{
}

Soldier::Soldier(Type type, int hp, int position) :
    type{type},
    hp{hp},
    position{position},
    is_in_fight{false}
{
}

Soldier::Soldier(const std::string& type, int hp, int position) :
    type{string_to_soldier_type(type)},
    hp{hp},
    position{position},
    is_in_fight{false}
{
}

Soldier::Type string_to_soldier_type(const std::string& str)
{
    if (str == "swordsman")
    {
        return Soldier::Type::swordsman;
    }
    else
    {
        return Soldier::Type::archer;
    }
}

std::string soldier_type_to_string(const Soldier::Type type)
{
    switch (type)
    {
        case Soldier::Type::swordsman:
            return "swordsman";
        case Soldier::Type::archer:
            return "archer";
        default:
            return "";
    }
}
