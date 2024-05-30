#include "eval.hpp"
#include <string>

int eval_mobility(const Engine& engine, const Side side);
int eval_control(const Engine& engine, const Side side);

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

    result += engine.get_turrets(mySide).size() * 200;
    result -= engine.get_turrets(other_side(mySide)).size() * 200;

	result += engine.get_legal_moves(mySide).size() * 100;
	result -= engine.get_legal_moves(other_side(mySide)).size() * 100;

	result += eval_mobility(engine, mySide);
	result -= eval_mobility(engine, other_side(mySide));

	result += eval_control(engine, mySide);
	result -= eval_control(engine, other_side(mySide));

    return result;
}

int eval_mobility(const Engine& engine, const Side side)
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

int eval_control(const Engine& engine, const Side side)
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
