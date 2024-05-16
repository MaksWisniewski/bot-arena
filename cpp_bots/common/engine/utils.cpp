#include "utils.hpp"

int distance(const std::pair<int, int>& x, const std::pair<int, int>& y)
{
    return std::abs(x.first- y.first) + std::abs(x.second - y.second);
}

std::size_t pair_hash::operator()(const std::pair<int, int> &p) const
{
    return (static_cast<std::size_t>(p.first) << 32) | static_cast<std::size_t>(p.second);
}

std::pair<int, int> parse_builiding_position(const std::string& position_string)
{
    int x, y;
    std::stringstream stream{position_string};
    stream >> x >> y;
    return {x, y};
}
