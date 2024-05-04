#include "soldier.hpp"

#include <string>

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

Soldier::Soldier(const Json& json) :
    type{string_to_soldier_type(json["type"])},
    hp{json["hp"]},
    position{json["position"]}
{
}
