#pragma once

#include "../common/engine/engine.hpp"

#include <string>
#include <unordered_map>

class MCTSNode
{
public:
    void update();
    std::string get_best_move();

private:
    // returns true if simulated ended with win
    bool simulate();

    // returns number of won games
    int expand();

    double score();
    bool is_leaf();

    int games_played;
    int games_won;
    std::unordered_map<std::string, MCTSNode> children;
};
