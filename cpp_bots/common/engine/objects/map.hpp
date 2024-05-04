#pragma once

#include <vector>

struct Map
{
    std::pair<int, int> size;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> obstacles;
};
