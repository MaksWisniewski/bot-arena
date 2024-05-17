#include "eval.hpp"

Eval::Type Eval_1::operator() (Engine& engine, const Side mySide) const {
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
