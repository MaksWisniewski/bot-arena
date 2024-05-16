#pragma once

#include <utility>
#include <sstream>
#include <cstdlib>

int distance(const std::pair<int, int>&, const std::pair<int, int>&);

struct pair_hash
{
    std::size_t operator()(const std::pair<int, int>&) const;
};

std::pair<int, int> parse_builiding_position(const std::string&);
