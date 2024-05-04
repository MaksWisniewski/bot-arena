#include "common/bot/bot.hpp"
#include "common/engine/engine.hpp"

#include <iostream>
#include <format>

class MinMaxBot : public Bot
{
    void preprocess() override
    {
        const Engine engine{arena_properties, side};
    }

    std::string make_move() override
    {
        const auto move = Bot::make_move();
        std::cerr << std::format("[minmax.cpp] my move: {0}\n", move);
        return move;
    }
};

int main()
{
    MinMaxBot bot;
    bot.run();
}
