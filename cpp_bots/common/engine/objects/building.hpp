#pragma once

#include <utility>

struct Building
{
    Building() = default;
    Building(const std::pair<int, int>&);

    operator std::pair<int, int>() const;

    std::pair<int, int> position;
};
