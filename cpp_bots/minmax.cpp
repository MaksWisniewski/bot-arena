#include "common/bot/bot.hpp"
#include "common/engine/engine.hpp"
#include "common/eval_func/eval.hpp"
#include "common/read_json.hpp"

#include <iostream>
#include <format>

#include <climits>
#include <algorithm>
#include <chrono>
#include <random>

#include <getopt.h>

class MinMaxBot : public Bot
{
public:
    MinMaxBot(Eval& _eval) : eval(_eval)
    {
        std::random_device rd;
        rng = std::mt19937{rd()};
    }

    Eval::Type search(const Engine& engine, Side side, int depth, Eval::Type alpha=LLONG_MIN, Eval::Type beta=LLONG_MAX)
    {
        if (std::chrono::high_resolution_clock::now() >= max_time)
        {
            is_timeout = true;
            return 0;
        }

        if(depth == 0 || engine.isWin())
            return eval(engine, this->side);

        bool isMaximizingPlayer = side == this->side;
        auto moves = engine.get_legal_moves(side);
        std::shuffle(moves.begin(), moves.end(), rng);

        if(isMaximizingPlayer)
        {
            Eval::Type result = LLONG_MIN;

            for(auto &move : moves)
            {
                if (is_timeout)
                {
                    return 0;
                }

                Engine temp_engine = engine;
                if(side == Side::left)
                    temp_engine.make_move(move, "W");
                else
                    temp_engine.make_move("W", move);

                result = std::max(result, search(temp_engine, other_side(side), depth-1, alpha, beta));

                if(result > beta) break;
                alpha = std::max(alpha, result);
            }
            return result;
        }
        else
        {
            Eval::Type result = LLONG_MAX;

            for(auto &move : moves)
            {
                if (is_timeout)
                {
                    return 0;
                }

                Engine temp_engine = engine;
                if(side == Side::left)
                    temp_engine.make_move(move, "W");
                else
                    temp_engine.make_move("W", move);

                result = std::min(result, search(temp_engine, other_side(side), depth-1, alpha, beta));

                if(result < alpha) break;
                beta = std::min(beta, result);
            }
            return result;
        }
    }

    std::string make_move() override
    {
        max_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds{move_timeout} - std::chrono::milliseconds{25};
        is_timeout = false;

        Engine engine{arena_properties};

        auto legal_moves = engine.get_legal_moves(side);
        std::shuffle(legal_moves.begin(), legal_moves.end(), rng);

        std::string bestMove = "W";

        int depth;
        for (depth = 0; not is_timeout; depth++)
        {
            std::string tmpBestMove = "";
            Eval::Type tmpBestResult = LLONG_MIN;

            for (auto &move : legal_moves)
            {
                if (is_timeout)
                {
                    std::cerr << std::format("[minmax] searched for max depth {0}, best move: {1}\n", depth, bestMove);
                    return bestMove;
                }

                Engine temp_engine = engine;
                if (side == Side::left)
                    temp_engine.make_move(move, "W");
                else
                    temp_engine.make_move("W", move);

                auto result = search(temp_engine, other_side(side), depth);

                if (result > tmpBestResult)
                {
                    tmpBestMove = move;
                    tmpBestResult = result;
                }
            }

            bestMove = tmpBestMove;
        }

        std::cerr << std::format("[minmax] searched for max depth {0}, best move: {1}\n", depth, bestMove);
        return bestMove;
    }

private:
    Eval& eval;
    std::chrono::system_clock::time_point max_time;
    bool is_timeout;
    std::mt19937 rng;
};

BetterEval get_eval_with_parameters(int argc, char** argv)
{
    option options[] = {
        {"eval-config", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int option;
    while ((option = getopt_long(argc, argv, "c:", options, nullptr)) != -1)
    {
        switch (option)
        {
            case 'c':
            {
                const auto eval_config_opt = read_json(optarg);
                if (not eval_config_opt)
                {
                    std::cerr << "[minmax] failed to read eval parameters from file: " << optarg << '\n';
                    break;
                }

                const auto eval_config = eval_config_opt.value();
                std::cerr << "[minmax] running with eval parameters: " << eval_config << '\n';
                return BetterEval{eval_config};
            }
        }
    }

    std::cerr << "[minmax] running with default eval parameters\n";
    return {};
}

int main(int argc, char** argv)
{
    auto eval = get_eval_with_parameters(argc, argv);
    for (;;)
    {
        MinMaxBot bot{eval};
        bot.run();
    }
}
