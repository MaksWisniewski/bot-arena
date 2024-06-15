#include "../common/bot/bot.hpp"
#include "../common/engine/engine.hpp"
#include "../common/eval_func/eval.hpp"
#include "../common/read_json.hpp"
#include "node.hpp"

#include <iostream>
#include <format>
#include <getopt.h>

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

struct ProgramArguments
{
    BetterEval eval;
    int max_simulation_length;
};

ProgramArguments parse_arguments(int argc, char** argv)
{
    option options[] = {
        {"eval-config", required_argument, 0, 'c'},
        {"sim-length", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

    ProgramArguments result{{}, MCTSNode::default_max_simulation_length};

    bool is_default_eval{true};

    int option;
    while ((option = getopt_long(argc, argv, "c:l:", options, nullptr)) != -1)
    {
        switch (option)
        {
            case 'c':
            {
                const auto eval_config_opt = read_json(optarg);
                if (not eval_config_opt)
                {
                    std::cerr << "[mcts] failed to read eval parameters from file: " << optarg << '\n';
                }
                else
                {
                    const auto eval_config = eval_config_opt.value();
                    std::cerr << "[mcts] running with eval parameters: " << eval_config << '\n';
                    result.eval = {eval_config};
                    is_default_eval = false;
                }
                break;
            }
            case 'l':
            {
                result.max_simulation_length = std::stoi(optarg);
                break;
            }
        }
    }

    if (is_default_eval)
    {
        std::cerr << "[mcts] running with default eval parameters\n";
    }

    return result;
}

int main(int argc, char** argv)
{
    auto [eval, max_simulation_length] = parse_arguments(argc, argv);
    for (;;)
    {
        MCTSBot bot{eval, max_simulation_length};
        bot.run();
    }
}
