#include "is_useless.hpp"

#include <array>
#include <random>

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

double get_random_number()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

// TODO: filtrować jakoś farmy
// TODO: check if is_useless isn't useless :)
bool is_useless(const std::string& move, const Path& path)
{
    if (move.front() == 'T' and not is_near_path(parse_builiding_position(move), path))
    {
        return true;
    }

    if (move.front() == 'F')
    {
        return get_random_number() < std::min(0.8, 0.02 * path.size());
    }

    return false;
}
