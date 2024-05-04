#include "map.hpp"

Map::Map(const Json& json) :
    size{json["map_size"]},
    path(json["path"]),
    obstacles(json["obstacles"])
{
}
