#include "node.hpp"
#include "../common/optimizations/is_useless.hpp"

#include <algorithm>
#include <random>
#include <cmath>

MCTSNode::MCTSNode(const Side side, const MCTSNode* parent) : side(side), games_played(0), games_won(0), parent(parent)
{
}

MCTSNode::Result MCTSNode::update(
    Engine& engine,
    const Side my_side,
    const Eval& eval,
    const std::chrono::system_clock::time_point& max_time,
    bool& is_timeout,
    int max_simulation_length)
{
    if (is_leaf())
    {
        return expand(engine, my_side, eval, max_time, is_timeout, max_simulation_length);
    }

    const auto move = select_move();

    if (side == Side::left)
    {
        engine.make_move(move, "W");
    }
    else
    {
        engine.make_move("W", move);
    }

    const auto result = children.at(move).update(engine, my_side, eval, max_time, is_timeout, max_simulation_length);

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
            return x.second.games_played < y.second.games_played;
        };

    // TODO: consider adding some randomness here
    return std::max_element(children.begin(), children.end(), compare)->first;
}

std::string MCTSNode::select_move() const
{
    auto compare =
        [](const auto& x, const auto& y)
        {
            return x.second.score() < y.second.score();
        };

    // TODO: consider adding some randomness here
    return std::max_element(children.begin(), children.end(), compare)->first;
}

MCTSNode::Result MCTSNode::expand(
    const Engine& engine,
    const Side my_side,
    const Eval& eval,
    const std::chrono::system_clock::time_point& max_time,
    bool& is_timeout,
    int max_simulation_length)
{
    if (std::chrono::high_resolution_clock::now() >= max_time)
    {
        is_timeout = true;
        return {0, 0};
    }

    if (engine.isWin())
    {
        const auto result = engine.isWin(my_side);

        games_played++;
        games_won += result;

        return {.games_played = 1, .games_won = result};
    }

    const auto legal_moves = engine.get_legal_moves(side);
    Result result{.games_played = static_cast<int>(legal_moves.size()), .games_won = 0};

    const auto p = engine.get_path();
    const Path path{p.begin(), p.end()};

    for (auto& move: legal_moves)
    {
        if (std::chrono::high_resolution_clock::now() >= max_time)
        {
            is_timeout = true;
            break;
        }

        if (is_useless(move, path))
        {
            continue;
        }

        Engine child_engine{engine};
        if (side == Side::left)
        {
            child_engine.make_move(move, "W");
        }
        else
        {
            child_engine.make_move("W", move);
        }

        MCTSNode child{other_side(side), this};
        result.games_won += child.simulate(child_engine, my_side, eval, max_simulation_length);
        children.insert({move, child});
    }

    games_played += result.games_played;
    games_won += result.games_won;

    return result;
}

std::string get_random_move(const std::vector<std::string>& moves)
{
    // TODO: consider non-uniform distribution
    std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<std::size_t> distribution(0, moves.size() - 1);

    std::size_t randomIndex = distribution(generator);
    return moves[randomIndex];
}

bool MCTSNode::simulate(Engine& engine, const Side my_side, const Eval& eval, int max_simulation_length)
{
    for (int i = 0; i < max_simulation_length; i++)
    {
        if (engine.isWin())
        {
            break;
        }

        const auto left_move = get_random_move(engine.get_legal_moves(Side::left));
        const auto right_move = get_random_move(engine.get_legal_moves(Side::right));

        engine.make_move(left_move, right_move);
    }

    const auto result = eval(engine, my_side) > 0;

    games_played++;
    games_won += result;

    return result;
}

double MCTSNode::score() const
{
    return static_cast<double>(games_won) / games_played + std::sqrt(2.0 * std::log(parent->games_played) / games_played);

    // return games_played;
    // return games_played == 0 ? 0.0 : static_cast<double>(games_won) / games_played;
}

bool MCTSNode::is_leaf() const
{
    return children.empty();
}
