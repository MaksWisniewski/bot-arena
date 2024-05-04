#include "engine.hpp"
#include "objects/soldier.hpp"

#include <iostream>

Engine::Engine(const Json& game_state, const Side player_to_move) :
    map{game_state["arena"]},
    game_parameters{game_state["arena"]["stats"]},
    players{
        {Side::left, game_state["players"]["left"]},
        {Side::right, game_state["players"]["right"]}
    },
    player_to_move{player_to_move}
{
    std::cerr << "map size: " << map.size.first << ' ' << map.size.second << '\n';
    for (auto [x, y] : map.path)
        std::cerr << '(' << x << ',' << y << ") ";
    std::cerr << '\n';
    for (auto [x, y] : map.obstacles)
        std::cerr << '(' << x << ',' << y << ") ";
    std::cerr << '\n';

    std::cerr << "swordsman: " <<
        game_parameters.soldiers[Soldier::Type::swordsman].max_hp << ' ' <<
        game_parameters.soldiers[Soldier::Type::swordsman].damage << ' ' <<
        game_parameters.soldiers[Soldier::Type::swordsman].range << ' ' <<
        game_parameters.soldiers[Soldier::Type::swordsman].cost << '\n';
    std::cerr << "archer: " <<
        game_parameters.soldiers[Soldier::Type::archer].max_hp << ' ' <<
        game_parameters.soldiers[Soldier::Type::archer].damage << ' ' <<
        game_parameters.soldiers[Soldier::Type::archer].range << ' ' <<
        game_parameters.soldiers[Soldier::Type::archer].cost << '\n';
    std::cerr << "farm: " << game_parameters.farm.gold << ' ' << game_parameters.farm.cost << '\n';
    std::cerr << "turret: " << game_parameters.turret.attack << ' ' << game_parameters.turret.range
     << ' ' << game_parameters.turret.cost << '\n';

    std::cerr << "left: " << players[Side::left].gold << ' ' << players[Side::left].income << '\n';
    std::cerr << "left farms: ";
    for (auto x : players[Side::left].farms)
        std::cerr << '(' << x.position.first << ',' << x.position.second << ") ";
    std::cerr << '\n';
    std::cerr << "left turrets: ";
    for (auto x : players[Side::left].turrets)
        std::cerr << '(' << x.position.first << ',' << x.position.second << ") ";
    std::cerr << '\n';
    std::cerr << "left soldiers: ";
    for (auto x : players[Side::left].soldiers)
        std::cerr << '(' << (int)x.type << ',' << x.hp << ',' << x.position << ") ";
    std::cerr << '\n';

    std::cerr << "right: " << players[Side::right].gold << ' ' << players[Side::right].income << '\n';
    std::cerr << "right farms: ";
    for (auto x : players[Side::right].farms)
        std::cerr << '(' << x.position.first << ',' << x.position.second << ") ";
    std::cerr << '\n';
    std::cerr << "right turrets: ";
    for (auto x : players[Side::right].turrets)
        std::cerr << '(' << x.position.first << ',' << x.position.second << ") ";
    std::cerr << '\n';
    std::cerr << "right soldiers: ";
    for (auto x : players[Side::right].soldiers)
        std::cerr << '(' << (int)x.type << ',' << x.hp << ',' << x.position << ") ";
    std::cerr << '\n';
}

void Engine::make_move(const Move&)
{
    // TODO: update game state
}

void Engine::undo_move()
{
    // TODO: update game state - undo last move
}
