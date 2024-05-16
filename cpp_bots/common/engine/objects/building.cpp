#include "building.hpp"

Building::Building(const std::pair<int, int>& position) : position(position)
{
}

Building::operator std::pair<int, int>() const
{
    return position;
}
