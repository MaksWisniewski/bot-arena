#include "../common/bot/bot.hpp"
#include "../common/engine/engine.hpp"
#include "node.hpp"

#include <iostream>
#include <chrono>
#include <format>

class MCTSBot : public Bot
{
public:
    MCTSBot(int max_simulation_length = MCTSNode::default_max_simulation_length) :
        max_simulation_length(max_simulation_length)
    {
        std::cerr << std::format("[mcts] running with max simulation length: {0}\n", max_simulation_length);
    }

    // TODO: fix it or get rid of it as it causes ready timeout
    void preprocess() override
    {
        const auto max_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds{ready_timeout} - std::chrono::milliseconds{2000};

        MCTSNode root{side};
        Engine engine{arena_properties};

        int number_of_simulations = 0;
        // TODO: fix milliseconds(1000)
        while (std::chrono::high_resolution_clock::now() < max_time)
        {
            root.update(engine, side, max_simulation_length);
            number_of_simulations++;
        }

        mean_simulation_duration = std::chrono::duration<double>{static_cast<double>(ready_timeout) / number_of_simulations};

        std::cerr << std::format("[mcts]: performed {0} simulations in {1} seconds\n", number_of_simulations, ready_timeout);
        std::cerr << std::format("[mcts]: mean simulation time: {0}\n", mean_simulation_duration);
    }

    std::string make_move() override
    {
        const auto max_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds{move_timeout} - 2 * mean_simulation_duration;

        MCTSNode root{side};
        Engine engine{arena_properties};

        int number_of_simulations = 0;
        while (std::chrono::high_resolution_clock::now() < max_time)
        {
            // TODO: check timeout during simulation, not only before starting it (as in minmax)
            root.update(engine, side, max_simulation_length);
            number_of_simulations++;
        }

        const auto move = root.get_best_move();

        std::cerr << std::format("[mcts] performed {0} simulations, best move: {1}\n", number_of_simulations, move);

        return move;
    }

private:
    int max_simulation_length;
    std::chrono::duration<double> mean_simulation_duration;
};

int main()
{
    for (;;)
    {
        MCTSBot bot;
        bot.run();
    }
}
