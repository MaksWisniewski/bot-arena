#include "common/bot/bot.hpp"
#include "common/engine/engine.hpp"

#include <iostream>
#include <format>

class MinMaxBot : public Bot
{
    void preprocess() override
    {
        std::cerr << "my side: " << side_to_string(side)
                  << ", opponents side: " << side_to_string(other_side(side)) << '\n';
    }

    std::string make_move() override
    {
        Engine engine{arena_properties};

        const auto move = "S swordsman";

        engine.make_move(move, "W");

        std::cerr << std::format("[minmax.cpp] my move: {0}\n", move);
        return move;
    }
};

int main()
{
    MinMaxBot bot;
    bot.run();
}
