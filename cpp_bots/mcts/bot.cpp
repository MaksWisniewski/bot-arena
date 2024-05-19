#include "../common/bot/bot.hpp"
#include "../common/engine/engine.hpp"
#include "node.hpp"

#include <iostream>

class MCTSBot : public Bot
{
public:
    MCTSBot(int max_number_of_simulations = 100, int max_simulation_length = MCTSNode::default_max_simulation_length) :
        max_number_of_simulations(max_number_of_simulations),
        max_simulation_length(max_simulation_length)
    {
        std::cerr << "MCTS running with max number of simulations: " << max_number_of_simulations
                  << " and max simulation length: " << max_simulation_length << '\n';
    }

    std::string make_move() override
    {
        MCTSNode root{side};
        Engine engine{arena_properties};

        for (int i = 0; i < max_number_of_simulations; i++)
        {
            root.update(engine, side, max_simulation_length);
        }
        const auto move = root.get_best_move();

        std::cerr << "[mcts.cpp]: " << move << '\n';

        return move;
    }

private:
    int max_number_of_simulations;
    int max_simulation_length;
};

int main()
{
    for (;;)
    {
        MCTSBot bot;
        bot.run();
    }
}
