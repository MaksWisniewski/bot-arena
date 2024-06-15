#include "../common/bot/bot.hpp"
#include "../common/engine/engine.hpp"
#include "../common/eval_func/eval.hpp"
#include "node.hpp"

#include <iostream>
#include <format>

class MCTSBot : public Bot
{
public:
    MCTSBot(Eval& eval, int max_simulation_length = MCTSNode::default_max_simulation_length) :
        eval{eval},
        max_simulation_length{max_simulation_length}
    {
        std::cerr << std::format("[mcts] running with max simulation length: {0}\n", max_simulation_length);
    }

    std::string make_move() override
    {
        const auto max_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds{move_timeout} - std::chrono::milliseconds{25};
        bool is_timeout{false};

        MCTSNode root{side};
        Engine engine{arena_properties};

        int number_of_simulations;
        for (number_of_simulations = 0; not is_timeout; number_of_simulations++)
        {
            root.update(engine, side, eval, max_time, is_timeout, max_simulation_length);
        }

        const auto move = root.get_best_move();

        std::cerr << std::format("[mcts] performed {0} simulations, best move: {1}\n", number_of_simulations, move);

        return move;
    }

private:
    Eval& eval;
    int max_simulation_length;
};

int main()
{
    BetterEval eval{};
    for (;;)
    {
        MCTSBot bot{eval, 40};
        bot.run();
    }
}
