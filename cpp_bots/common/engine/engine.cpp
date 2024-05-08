#include "engine.hpp"
#include "objects/soldier.hpp"

#include <iostream>
#include <algorithm>

Engine::Engine(const Json& game_state) :
    map{game_state["arena"]},
    game_parameters{game_state["arena"]["stats"]},
    players{
        {Side::left, game_state["players"]["left"]},
        {Side::right, game_state["players"]["right"]}
    }
{
}

int distance(const std::pair<int, int>& x, const std::pair<int, int>& y)
{
    return std::abs(x.first- y.first) + std::abs(x.second - y.second);
}

void Engine::make_move(const Move& left_move, const Move& right_move)
{
    last_moves = {left_move, right_move};

    // TODO: compare python code with README.md -> possible differences !!!!!!!! (not good)

    // update soldiers
        // fight
        // move

    // shoot turrets
    for (auto& [side, player] : players)
    {
        auto& opponent_soldiers = players[other_side(side)].soldiers;
        for (auto& turret : player.turrets)
        {
            // choose target
            // assumption: soldiers are sorted in ascending order by their distance to the opponent's base
            auto target = std::find_if(
                opponent_soldiers.begin(),
                opponent_soldiers.end(),
                [this, &turret](const Soldier& soldier) {
                    return distance(turret.position, map.path[soldier.position]) <= game_parameters.turret.range;
                });

            if (target == opponent_soldiers.end())
            {
                continue;
            }

            std::cerr << "[turret at " << turret.position.first << ',' << turret.position.second
                        << "] Chosen target: " << target-> position
                        << " with distance " << distance(turret.position, map.path[target->position]) << '\n';
            // shoot
            target->hp -= game_parameters.turret.attack;
        }
    }

    // clear dead soldiers
    for (auto& [side, player] : players)
    {
        auto& soldiers = player.soldiers;
        std::cerr << side_to_string(side) << " soldiers before: " << soldiers.size();
        soldiers.erase(
            std::remove_if(
                soldiers.begin(),
                soldiers.end(),
                [](const Soldier& soldier){ return soldier.hp <= 0; }),
            soldiers.end());
        std::cerr << ", after: " << soldiers.size() << '\n';
    }

    // execute player actions

    // produce gold

    // update income
}

void Engine::undo_move()
{
    // TODO: update game state - undo last move
}
