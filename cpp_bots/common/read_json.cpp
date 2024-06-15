#include "read_json.hpp"

#include <fstream>

std::optional<Json> read_json(const std::string& file_name)
{
    std::ifstream file{file_name};

    if (not file.is_open())
    {
        return std::nullopt;
    }

    Json result;
    file >> result;

    file.close();

    return result;
}
