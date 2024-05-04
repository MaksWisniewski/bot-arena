#pragma once

#include "../json.hpp"
#include "../side.hpp"

#include "objects/map.hpp"
#include "objects/game_parameters.hpp"
#include "objects/player.hpp"

#include <unordered_map>

class Engine
{
public:
    Engine(const Json&, Side);

    using Move = std::string;

    void make_move(const Move&);
    void undo_move();

    // TODO: game state getters

private:
    Map map;
    GameParameters game_parameters;
    std::unordered_map<Side, Player> players;

    Move last_move;
    Side player_to_move;
};
