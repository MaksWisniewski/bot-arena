#pragma once

#include "json.hpp"
#include "side.hpp"

class Engine
{
public:
    Engine(const Json&, Side);

    void make_move(const std::string&);
    void undo_move();

    // TODO: game state getters

private:
    // TODO: game state
};
