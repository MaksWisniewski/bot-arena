#include "engine.hpp"
#include "objects/soldier.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <format>

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

void Engine::make_move(const std::string& left_move, const std::string& right_move)
{
    last_moves = {left_move, right_move};

    fight_soldiers();
    move_soldiers();
    shoot_turrets();
    clear_dead_soldiers();
    execute_player_actions(left_move, right_move);
    update_gold_and_income();
}

void Engine::undo_move()
{
    #warning Engine::undo_move not implemented
    // TODO: update game state - undo last move
}

std::vector<std::string> Engine::get_legal_moves(const Side side)
{
    std::vector<std::string> legal_moves{"W"};

    const auto& player = players[side];

    const int base = side == Side::left ? 0 : map.path.size() - 1;
    for (auto& [type, parameters] : game_parameters.soldiers)
    {
        // TODO: check if base is empty - not trivial because soldiers move first and then player actions are performed
        // checking this would require to simulate part of a turn - not really efficient
        // -> it is easier not to check it, hopefully this would not harm us that much
        if (parameters.cost <= player.gold /*and (player.soldiers.empty() or player.soldiers.back().position != base)*/)
        {
            legal_moves.push_back("S " + soldier_type_to_string(type));
        }
    }

    const auto empty_cells = get_empty_cells();
    if (game_parameters.farm.cost <= player.gold)
    {
        for (auto& empty_cell : empty_cells)
        {
            legal_moves.push_back(std::format("F {0} {1}", empty_cell.first, empty_cell.second));
        }
    }

    if (game_parameters.turret.cost <= player.gold)
    {
        for (auto& empty_cell : empty_cells)
        {
            legal_moves.push_back(std::format("T {0} {1}", empty_cell.first, empty_cell.second));
        }
    }

    return legal_moves;
}

struct pair_hash
{
    std::size_t operator () (const std::pair<int, int> &p) const
    {
        return (static_cast<std::size_t>(p.first) << 32) | static_cast<std::size_t>(p.second);
    }
};

std::vector<std::pair<int, int>> Engine::get_empty_cells()
{
    std::unordered_set<std::pair<int, int>, pair_hash> non_empty_cells;
    non_empty_cells.insert(map.path.begin(), map.path.end());
    non_empty_cells.insert(map.obstacles.begin(), map.obstacles.end());

    for (auto& [side, player] : players)
    {
        non_empty_cells.insert(player.farms.begin(), player.farms.end());
        non_empty_cells.insert(player.turrets.begin(), player.turrets.end());
    }

    std::vector<std::pair<int, int>> empty_cells{};
    for (int x = 0; x < map.size.first; x++)
    {
        for (int y = 0; y < map.size.second; y++)
        {
            const std::pair<int, int> cell{x,y};
            if (not non_empty_cells.contains(cell))
            {
                empty_cells.push_back(cell);
            }
        }
    }

    return empty_cells;
}

std::vector<Building> Engine::get_farms(const Side side)
{
    return players[side].farms;
}

std::vector<Building> Engine::get_turrets(const Side side)
{
    return players[side].turrets;
}

std::vector<Soldier> Engine::get_soldiers(const Side side)
{
    return players[side].soldiers;
}

void Engine::fight_soldiers()
{
    for (auto& [side, player] : players)
    {
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
}

void Engine::move_soldiers()
{
    for (auto& [side, player] : players)
    {
        int step = side == Side::left ? 1 : -1;
        for (auto i = 0u; i < player.soldiers.size(); i++)
        {
            auto& soldier = player.soldiers[i];
            if (soldier.is_in_fight)
            {
                std::cerr << side_to_string(side) << " soldier at " << soldier.position << " not moved - is in fight\n";
                continue;
            }

            int new_position = soldier.position + step;
            if (i == 0 or new_position != player.soldiers[i-1].position)
            {
                std::cerr << side_to_string(side) << " soldier moved from " << soldier.position
                          << " to " << new_position << '\n';
                soldier.position = new_position;
            }
            else
            {
                std::cerr << side_to_string(side) << " soldier at " << soldier.position << " not moved - is blocked\n";
            }
        }
    }
}

void Engine::shoot_turrets()
{
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
}

void Engine::clear_dead_soldiers()
{
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
}

void Engine::update_gold_and_income()
{
    for (auto& [side, player] : players)
    {
        std::cerr << side_to_string(side) << " gold, income before: " << player.gold << ' ' << player.income;

        player.gold += player.income;
        player.income = player.farms.size() * game_parameters.farm.gold + game_parameters.passive_gold;

        std::cerr << "; after: " << player.gold << ' ' << player.income << '\n';
    }
}

std::pair<int, int> parse_builiding_position(const std::string& position_string)
{
    int x, y;
    std::stringstream stream{position_string};
    stream >> x >> y;
    return {x, y};
}

void Engine::execute_player_actions(const std::string& left_move, const std::string& right_move)
{
    auto wants_to_and_can_build =
        [this](const Side side, const std::string& move)
        {
            return (move.front() == 'F' and players[side].gold >= game_parameters.farm.cost) or
                   (move.front() == 'T' and players[side].gold >= game_parameters.turret.cost);
        };

    if (wants_to_and_can_build(Side::left, left_move) and wants_to_and_can_build(Side::right, right_move) and
        parse_builiding_position(left_move.substr(2)) == parse_builiding_position(right_move.substr(2)))
    {
        return;
    }

    execute_action(Side::left, left_move);
    execute_action(Side::right, right_move);
}

void Engine::execute_action(const Side side, const std::string& action)
{
    if (action.front() == 'W')
    {
        return;
    }

    const auto action_parameters = action.substr(2);
    switch (action.front())
    {
        case 'F': return build_farm(side, action_parameters);
        case 'T': return build_turret(side, action_parameters);
        case 'S': return spawn_soldier(side, action_parameters);
        default : return;
    }
}

void Engine::build_farm(const Side side, const std::string& position)
{
    // TODO: check if empty cell
    auto& player = players[side];

    if (player.gold < game_parameters.farm.cost)
    {
        return;
    }

    player.gold -= game_parameters.farm.cost;
    player.farms.push_back(Building{parse_builiding_position(position)});

    std::cerr << side_to_string(side) << " build farm at " << player.farms.back().position.first << ' ' << player.farms.back().position.second << '\n';
}

void Engine::build_turret(const Side side, const std::string& position)
{
    // TODO: check if empty cell
    auto& player = players[side];

    if (player.gold < game_parameters.turret.cost)
    {
        return;
    }

    player.gold -= game_parameters.turret.cost;
    player.turrets.push_back(Building{parse_builiding_position(position)});

    std::cerr << side_to_string(side) << " build turret at " << player.turrets.back().position.first << ' ' << player.turrets.back().position.second << '\n';
}

void Engine::spawn_soldier(const Side side, const std::string& type)
{
    auto& player = players[side];
    const auto soldier_type = string_to_soldier_type(type);
    const auto soldier_parameters = game_parameters.soldiers[soldier_type];

    if (player.gold < soldier_parameters.cost)
    {
        return;
    }

    const int position = side == Side::left ? 0 : map.path.size() - 1;
    if (not player.soldiers.empty() and player.soldiers.back().position == position)
    {
        return;
    }

    const Soldier soldier{soldier_type, soldier_parameters.max_hp, position};

    player.gold -= soldier_parameters.cost;
    player.soldiers.push_back(soldier);

    std::cerr << side_to_string(side) << " spawn soldier at " << player.soldiers.back().position << '\n';
}
