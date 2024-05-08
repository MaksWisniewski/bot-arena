#pragma once

#include <string>

enum class Side : bool
{
    left,
    right
};

Side other_side(Side);
Side string_to_side(const std::string&);
std::string side_to_string(Side);
