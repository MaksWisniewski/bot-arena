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
    std::cerr << "passive_gold: " << game_parameters.passive_gold << "\n";
}

int distance(const std::pair<int, int>& x, const std::pair<int, int>& y)
{
    return std::abs(x.first- y.first) + std::abs(x.second - y.second);
}

void Engine::make_move(const Move& left_move, const Move& right_move)
{
    last_moves = {left_move, right_move};

    // update soldiers
    // fight
    for (auto& [side, player] : players)
    {
        // set is_in_fight to false for all soldiers
        std::for_each(
            player.soldiers.begin(),
            player.soldiers.end(),
            [](Soldier& soldier){ soldier.is_in_fight = false; });

        auto& opponent_soldiers = players[other_side(side)].soldiers;
        if (opponent_soldiers.empty())
        {
            continue;
        }
        auto& target = opponent_soldiers.front();

        std::for_each(
            player.soldiers.begin(),
            player.soldiers.end(),
            [this, &target, side](Soldier& soldier)
            {
                const auto& soldier_parameters = game_parameters.soldiers[soldier.type];
                if (abs(soldier.position - target.position) <= soldier_parameters.range)
                {
                    soldier.is_in_fight = true;
                    target.hp -= soldier_parameters.damage;
                    std::cerr << side_to_string(side) << " soldier at " << soldier.position << " attacks "
                              << side_to_string(other_side(side)) << " soldier at " << target.position << '\n';
                }
            });
    }
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
                [this, &turret](const Soldier& soldier)
                {
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

    // produce gold and update income
    for (auto& [side, player] : players)
    {
        std::cerr << side_to_string(side) << " gold, income before: " << player.gold << ' ' << player.income;

        player.gold += player.income;
        player.income = player.farms.size() * game_parameters.farm.gold + game_parameters.passive_gold;

        std::cerr << "; after: " << player.gold << ' ' << player.income << '\n';
    }
}

void Engine::undo_move()
{
    // TODO: update game state - undo last move
}
