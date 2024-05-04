#pragma once

#include "../../json.hpp"

#include <vector>

struct Map
{
    Map(const Json&);

    std::pair<int, int> size;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> obstacles;
};
