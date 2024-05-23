#pragma once

#include "../json.hpp"
#include "../side.hpp"

#include "objects/map.hpp"
#include "objects/game_parameters.hpp"
#include "objects/player.hpp"

#include <unordered_map>

class Engine
{
public:
    Engine(const Json&);

    // TODO:
    // - more getters for state
    void make_move(const std::string& left_move, const std::string& right_move);
    void undo_move();
    bool isWin() const;
    bool isWin(Side) const;

    std::vector<std::string> get_legal_moves(Side) const;

    std::vector<std::pair<int, int>> get_empty_cells() const;
    std::vector<std::pair<int, int>> get_path() const;

    std::vector<Building> get_farms(Side) const;
    std::vector<Building> get_turrets(Side) const;
    std::vector<Soldier> get_soldiers(Side) const;

private:
    void fight_soldiers();
    void move_soldiers();
    void shoot_turrets();
    void clear_dead_soldiers();
    void execute_player_actions(const std::string&, const std::string&);
    void update_gold_and_income();

    void execute_action(Side, const std::string&);
    void build_farm(Side, const std::string&);
    void build_turret(Side, const std::string&);
    void spawn_soldier(Side, const std::string&);

    Map map;
    GameParameters game_parameters;
    std::unordered_map<Side, Player> players;

    std::pair<std::string, std::string> last_moves;
};
