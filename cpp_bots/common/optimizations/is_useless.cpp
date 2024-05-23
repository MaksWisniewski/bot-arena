#include "is_useless.hpp"

#include <array>

std::pair<int, int> operator + (const std::pair<int, int>& x, const std::pair<int, int>& y)
{
    return {x.first + y.first, x.second + y.second};
}

bool is_near_path(const std::pair<int, int>& position, const Path& path)
{
    static const std::array<std::pair<int, int>, 4> delta{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};

    for (auto d : delta)
    {
        if (path.contains(position + d))
        {
            return true;
        }
    }

    return false;
}

// To powinno bardziej oddać w minmax'ie
// TODO: filtrować jakoś farmy
bool is_useless(const std::string& move, const Path& path)
{
    if (move.front() == 'T' and not is_near_path(parse_builiding_position(move), path))
    {
        return true;
    }

    static bool useless_farm = true;
    if (move.front() == 'F')
    {
        return useless_farm ^= 1;
    }

    return false;
}
