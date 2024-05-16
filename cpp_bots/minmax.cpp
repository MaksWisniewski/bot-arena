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

        const auto legal_moves = engine.get_legal_moves(side);

        std::cerr << "legal moves: ";
        for (auto move : legal_moves)
        {
            std::cerr << move << ", ";
        }
        std::cerr << '\n';

        // const auto move = side == Side::left ? "T 3 1" : "F 3 1";
        // const auto move = "T 3 1";
        // const auto move = "F 3 1";
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
