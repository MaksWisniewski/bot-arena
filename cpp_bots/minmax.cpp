#include "common/bot.hpp"

#include <iostream>
#include <format>

class MinMaxBot : public Bot
{
    void preprocess() override
    {
        std::cerr << "path: " << arena_properties["arena"]["path"] << '\n';
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
