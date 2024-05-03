#include "bot.hpp"

#include <iostream>
#include <format>

class MinMaxBot : public Bot
{
    void preprocess() override
    {
        std::cerr << std::format("[minmax.cpp] my side: {0}\narena:\n{1}\n", side == Side::left ? "left" : "right", arena_properties);
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
