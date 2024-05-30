#pragma once

#include "../common/engine/engine.hpp"

#include <string>
#include <chrono>
#include <unordered_map>

class MCTSNode
{
public:
    static constexpr int default_max_simulation_length = 100;

    MCTSNode(Side, const MCTSNode* parent = nullptr);

    struct Result
    {
        int games_played;
        int games_won;
    };

    Result update(
        Engine&,
        Side,
        const std::chrono::system_clock::time_point& max_time,
        bool& is_timeout,
        int max_simulation_length = default_max_simulation_length);

    // used to get best move after performing simulations
    std::string get_best_move() const;

private:
    // used to select the node to expand while performing simulations
    std::string select_move() const;

    Result expand(
        const Engine&,
        Side,
        const std::chrono::system_clock::time_point& max_time,
        bool& is_timeout,
        int max_simulation_length);

    // returns true if simulation ended with win for given side
    bool simulate(Engine&, Side, int max_simulation_length);

    double score() const;
    bool is_leaf() const;

    Side side;
    int games_played;
    int games_won;

    std::unordered_map<std::string, MCTSNode> children;
    const MCTSNode* parent;
};
