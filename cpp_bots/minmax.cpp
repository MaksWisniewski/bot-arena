// itearative Deepening
// warning moze byc tak ze potrzebne jest splamietyywanie?

#include "common/bot/bot.hpp"
#include "common/engine/engine.hpp"
#include "common/eval_func/eval.hpp"
// #include "common/optimizations/is_useless.hpp"

#include <iostream>
#include <format>
#include <climits>
#include <algorithm>
#include <chrono>

const int MAXDEPTH = 5;

class MinMaxBot : public Bot
{
public:
    MinMaxBot(Eval& _eval) : eval(_eval) {};

    Eval::Type search(const Engine& engine, Side side, int depth=MAXDEPTH, Eval::Type alpha=LLONG_MIN, Eval::Type beta=LLONG_MAX)
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
        std::random_shuffle(moves.begin(), moves.end());

        // const auto p = engine.get_path();
        // const Path path{p.begin(), p.end()};

        if(isMaximizingPlayer)
        {
            Eval::Type result = LLONG_MIN;

            for(auto &move : moves)
            {
                // if (is_useless(move, path))
                // {
                //     continue;
                // }

                if (is_timeout)
                {
                    return 0;
                }

                Engine temp_engine = engine; // kopia engina
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
                // if (is_useless(move, path))
                // {
                //     continue;
                // }

                if (is_timeout)
                {
                    return 0;
                }

                Engine temp_engine = engine; // kopia engina
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
        // TODO: probably can be done better
        max_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds{move_timeout} - std::chrono::milliseconds{25};
        is_timeout = false;

        Engine engine{arena_properties};

        auto legal_moves = engine.get_legal_moves(side);
        std::random_shuffle(legal_moves.begin(), legal_moves.end());

        // const auto p = engine.get_path();
        // const Path path{p.begin(), p.end()};

        std::string bestMove = "W";

        int depth;
        for (depth = 0; not is_timeout; depth++)
        {
            std::string tmpBestMove = "";
            Eval::Type tmpBestResult = LLONG_MIN;

            for (auto &move : legal_moves)
            {
                // if (is_useless(move, path))
                // {
                //     continue;
                // }

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
};

int main()
{
    // Eval_1 eval{};
    BetterEval eval{};
    for (;;)
    {
        MinMaxBot bot{eval};
        bot.run();
    }
}
