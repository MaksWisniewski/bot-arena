#pragma once

#include "../json.hpp"
#include "../side.hpp"

#include "objects/map.hpp"
#include "objects/game_parameters.hpp"
#include "objects/player.hpp"

#include <unordered_map>
#include <utility>

class Engine
{
public:
    Engine(const Json&);

    using Move = std::string;

    void make_move(const Move& left_move, const Move& right_move);
    void undo_move();

    // TODO: game state getters

private:
    void fight_soldiers();
    void move_soldiers();
    void shoot_turrets();
    void clear_dead_soldiers();
    void execute_player_action();
    void update_gold_and_income();

    Map map;
    GameParameters game_parameters;
    std::unordered_map<Side, Player> players;

    std::pair<Move, Move> last_moves;
};
