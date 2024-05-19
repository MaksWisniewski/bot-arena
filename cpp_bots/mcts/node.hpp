#pragma once

#include "../common/engine/engine.hpp"

#include <string>
#include <unordered_map>

class MCTSNode
{
public:
    static constexpr int default_max_simulation_length = 150;

    MCTSNode(Side);

    struct Result
    {
        int games_played;
        int games_won;
    };

    Result update(Engine&, Side, int max_simulation_length = default_max_simulation_length);
    std::string get_best_move() const;

private:
    Result expand(const Engine&, Side, int max_simulation_length);

    // returns true if simulation ended with win for given side
    bool simulate(Engine&, Side, int max_simulation_length);

    double score() const;
    bool is_leaf() const;

    Side side;
    int games_played;
    int games_won;
    std::unordered_map<std::string, MCTSNode> children;
};
