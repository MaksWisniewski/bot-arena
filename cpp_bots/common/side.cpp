#include "side.hpp"

Side other_side(const Side side)
{
    return static_cast<Side>(not static_cast<bool>(side));
}

Side string_to_side(const std::string& str)
{
    if (str == "left")
    {
        return Side::left;
    }
    else
    {
        return Side::right;
    }
}

std::string side_to_string(const Side side)
{
    switch (side)
    {
        case Side::left:
            return "left";
        case Side::right:
            return "right";
        default:
            return "";
    }
}
