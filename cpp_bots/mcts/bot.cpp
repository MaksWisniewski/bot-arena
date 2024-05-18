#include "../common/bot/bot.hpp"
#include "../common/engine/engine.hpp"
#include "node.hpp"

#include <iostream>

class MCTSBot : public Bot
{
public:
    std::string make_move() override
    {
        root.update();
        const auto move = root.get_best_move();

        std::cerr << "[mcts.cpp]: " << move << '\n';

        return move;
    }

private:
    MCTSNode root;
};

int main()
{
    for (;;)
    {
        MCTSBot bot;
        bot.run();
    }
}
