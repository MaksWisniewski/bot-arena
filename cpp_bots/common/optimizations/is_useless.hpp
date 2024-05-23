#pragma once

#include "../engine/utils.hpp"

#include <string>
#include <unordered_set>

using Path = std::unordered_set<std::pair<int, int>, pair_hash>;

bool is_useless(const std::string&, const Path&);
