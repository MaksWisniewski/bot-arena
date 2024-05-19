#include "node.hpp"

#include <algorithm>
#include <iostream>

MCTSNode::MCTSNode(const Side side) : side(side), games_played(0), games_won(0)
{
}

MCTSNode::Result MCTSNode::update(Engine& engine, const Side my_side, int max_simulation_length)
{
    if (is_leaf())
    {
        return expand(engine, my_side, max_simulation_length);
    }

    const auto move = get_best_move();

    std::cerr << "MCTS: chosen child node: " << move << '\n';

    if (side == Side::left)
    {
        engine.make_move(move, "W");
    }
    else
    {
        engine.make_move("W", move);
    }

    const auto result = children.at(move).update(engine, my_side, max_simulation_length);

    games_played += result.games_played;
    games_won += result.games_won;

    return result;
}

std::string MCTSNode::get_best_move() const
{
    if (is_leaf())
    {
        return "W";
    }

    auto compare =
        [](const auto& x, const auto& y)
        {
            return x.second.score() < y.second.score();
        };

    // TODO: consider adding some randomness here
    return std::max_element(children.begin(), children.end(), compare)->first;
}

MCTSNode::Result MCTSNode::expand(const Engine& engine, const Side my_side, int max_simulation_length)
{
    std::cerr << "MCTS: expanding node\n";

    const auto legal_moves = engine.get_legal_moves(side);

    Result result{.games_played = static_cast<int>(legal_moves.size()), .games_won = 0};

    for (auto& move: legal_moves)
    {
        Engine child_engine{engine};
        if (side == Side::left)
        {
            child_engine.make_move(move, "W");
        }
        else
        {
            child_engine.make_move("W", move);
        }

        MCTSNode child{other_side(side)};

        std::cerr << "MCTS: simulating game for child node: " << move << '\n';

        result.games_won += child.simulate(child_engine, my_side, max_simulation_length);

        children.insert({move, child});
    }

    games_played += result.games_played;
    games_won += result.games_won;

    return result;
}

bool MCTSNode::simulate(Engine& engine, const Side my_side, int max_simulation_length)
{
    for (int i = 0; i < max_simulation_length; i++)
    {
        if (engine.isWin())
        {
            break;
        }

        // TODO: random
        const auto left_move = engine.get_legal_moves(Side::left).front();
        const auto right_move = engine.get_legal_moves(Side::right).front();

        engine.make_move(left_move, right_move);
    }

    // TODO: use evaluation function to check who wins
    const auto result = engine.isWin(my_side);

    games_played++;
    games_won += result;

    std::cerr << "MCTS: simulation result: " << side_to_string(result ? my_side : other_side(my_side)) << " won\n";

    return result;
}

double MCTSNode::score() const
{
    // TODO: implement better score function
    return games_played == 0 ? 0.0 : static_cast<double>(games_won) / games_played;
}

bool MCTSNode::is_leaf() const
{
    return children.empty();
}
