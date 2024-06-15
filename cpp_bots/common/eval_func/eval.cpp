#include "eval.hpp"
#include "../optimizations/is_useless.hpp" // for Path - maybe it should be extracted

#include <algorithm>
#include <iostream>

Eval::Type eval_mobility(const Engine&, const Side);
Eval::Type eval_control(const Engine&, const Side);

Eval::Type Eval_1::operator() (const Engine& engine, const Side mySide) const {
    Type result = 0;

    if(engine.isWin(mySide))
        result += 1e7;
    else if(engine.isWin(other_side(mySide)))
        result -= 1e7;

    result += engine.get_farms(mySide).size() * 500;
    result -= engine.get_farms(other_side(mySide)).size() * 100;

    // todo odleglosc od bazy
    result += engine.get_soldiers(mySide).size() * 160;
    result -= engine.get_soldiers(other_side(mySide)).size() * 300;

    result += engine.get_turrets(mySide).size() * 200;
    result -= engine.get_turrets(other_side(mySide)).size() * 200;

    return result;
}

Eval::Type Eval_2::operator() (const Engine& engine, const Side mySide) const {
    Type result = 0;

    if(engine.isWin(mySide))
        result += 1e7;
    else if(engine.isWin(other_side(mySide)))
        result -= 1e7;

    result += engine.get_farms(mySide).size() * 500;
    result -= engine.get_farms(other_side(mySide)).size() * 100;

    // todo odleglosc od bazy
    result += engine.get_soldiers(mySide).size() * 160;
    result -= engine.get_soldiers(other_side(mySide)).size() * 300;

    result += engine.get_turrets(mySide).size() * 800;
    result -= engine.get_turrets(other_side(mySide)).size() * 800;

	result += engine.get_legal_moves(mySide).size() * 100;
	result -= engine.get_legal_moves(other_side(mySide)).size() * 100;

	result += eval_mobility(engine, mySide);
	result -= eval_mobility(engine, other_side(mySide));

	result += eval_control(engine, mySide);
	result -= eval_control(engine, other_side(mySide));

    return result;
}

Eval::Type eval_mobility(const Engine& engine, const Side side)
{
    auto legal_moves = engine.get_legal_moves(side);

    const int weight_wait = 1;
    const int weight_soldier = 5;
    const int weight_farm = 3;
    const int weight_turret = 4;

    int mobility_score = 0;

    for (const auto& move : legal_moves)
    {
        if (move == "W")
        {
            mobility_score += weight_wait;
        }
        else if (move.substr(0, 2) == "S ")
        {
            mobility_score += weight_soldier;
        }
        else if (move.substr(0, 2) == "F ")
        {
            mobility_score += weight_farm;
        }
        else if (move.substr(0, 2) == "T ")
        {
            mobility_score += weight_turret;
        }
    }

    return mobility_score;
}

Eval::Type eval_control(const Engine& engine, const Side side)
{
    const auto& soldiers = engine.get_soldiers(side);

	std::pair<int, int> start_position; // maksymalizujemy odleglosc od startu

	// if (side_to_string(side) == std::string("left")) {
	if (side == Side::right) { // prawy czy lewy gracz zaczyna od konca sciezki?
		start_position = engine.get_path().back();
	}
	else {
		start_position = engine.get_path().front();
	}

    int control_score = 0;

    for (const auto& soldier : soldiers)
    {
		std::pair<int, int> soldier_position = engine.get_path()[soldier.position];
        control_score += std::abs(start_position.first - soldier_position.first) + std::abs(start_position.second - soldier_position.second);
    }

    return control_score;
}

/*---------------------------------- BetterEval ----------------------------------*/

Eval::Type eval_income(const Engine& engine, const Side side)
{
    const auto game_parameters = engine.get_game_parameters();
    const auto max_cost = std::max({
        game_parameters.farm.cost,
        game_parameters.turret.cost,
        game_parameters.soldiers.at(Soldier::Type::swordsman).cost,
        game_parameters.soldiers.at(Soldier::Type::archer).cost});

    const auto min_cost = std::min({
        game_parameters.farm.cost,
        game_parameters.turret.cost,
        game_parameters.soldiers.at(Soldier::Type::swordsman).cost,
        game_parameters.soldiers.at(Soldier::Type::archer).cost});

    const auto income = std::min(engine.get_income(side), max_cost);
    const auto map_size = engine.get_map_size();

    auto result = income * (2 * max_cost - income) * std::sqrt(map_size.first + map_size.second) / min_cost;
    // std::cerr << "income: " << result << '\n';
    return result;
}

Eval::Type eval_turrets(const Engine& engine, const Side side)
{
    Eval::Type result = 0;

    const auto p = engine.get_path();
    const Path path{p.begin(), p.end()};

    const auto game_parameters = engine.get_game_parameters();
    const auto range = game_parameters.turret.range;
    const auto attack = game_parameters.turret.attack;

    for (auto& turret : engine.get_turrets(side))
    {
        for (int x = -range; x <= range; x++)
        {
            for (int y = -range; y <= range; y++)
            {
                if (path.contains({turret.position.first + x, turret.position.second + y}))
                {
                    result += attack;
                }
            }
        }
    }

    const auto map_size = engine.get_map_size();
    result /= std::sqrt(map_size.first + map_size.second);
    // std::cerr << "turrets: " << result << '\n';
    return result;
}

Eval::Type BetterEval::operator() (const Engine& engine, const Side mySide) const {

    const auto otherSide = other_side(mySide);

    if(engine.isWin(mySide))
    {
        return 1LL << 30;
    }
    else if(engine.isWin(otherSide))
    {
        return -(1LL << 30);
    }

    Type result = 0;

    result += eval_income(engine, mySide) * income_weight;
    result -= eval_income(engine, otherSide) * income_weight;

    result += eval_control(engine, mySide) * control_weight;
    result -= eval_control(engine, otherSide) * control_weight;

    result += eval_turrets(engine, mySide) * turrets_weight;
    result -= eval_turrets(engine, otherSide) * turrets_weight;

    return result;
}

BetterEval::BetterEval(const Json& json) :
    income_weight{json["income_weight"]},
    control_weight{json["control_weight"]},
    turrets_weight{json["turrets_weight"]}
{
}
