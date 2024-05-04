#pragma once

#include <utility>

struct Building
{
    Building() = default;
    Building(const std::pair<int, int>&);

    std::pair<int, int> position;
};
