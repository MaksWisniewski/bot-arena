#pragma once

#include "json.hpp"

#include <optional>
#include <string>

std::optional<Json> read_json(const std::string& file_name);
