#include "common/bot/bot.hpp"
#include "common/engine/engine.hpp"
#include "common/eval_func/eval.hpp" 

#include <iostream>
#include <format>
#include <climits>
#include <algorithm>

const int MAXDEPTH = 5;

class MinMaxBot : public Bot
{   
    public:
    Eval eval;
    MinMaxBot(Eval _eval) : eval(_eval) {};

    int64_t search(Engine &engine, Side side, int depth=MAXDEPTH, int64_t alpha=LLONG_MIN, int64_t beta=LLONG_MAX)
    {
        if(depth == 0 || engine.isWin())    return eval(engine, this->side);

        bool isMaximizingPlayer = side == this->side;
        auto moves = engine.get_legal_moves(side);
        std::random_shuffle(moves.begin(), moves.end());

        if(isMaximizingPlayer)
        {
            int64_t result = LLONG_MIN;

            for(auto &mov : moves)
            {
                Engine temp_engine = engine; // kopia engina                
                if(side == Side::left)
                    temp_engine.make_move(mov, "W");
                else
                    temp_engine.make_move("W", mov);

                result = std::max(result, search(temp_engine, other_side(side), depth-1, alpha, beta));

                if(result > beta) break;
                alpha = std::max(alpha, result);
            }
            return result;
        }
        else
        {
            int64_t result = LLONG_MAX;

            for(auto &mov : moves)
            {
                Engine temp_engine = engine; // kopia engina                
                if(side == Side::left)
                    temp_engine.make_move(mov, "W");
                else
                    temp_engine.make_move("W", mov);

                result = std::min(result, search(temp_engine, other_side(side), depth-1, alpha, beta));

                if(result < alpha) break;
                beta = std::min(beta, result);
            }
            return result;
        } 
    }

    std::string make_move() override
    {
        Engine engine{arena_properties};

        auto legal_moves = engine.get_legal_moves(side);
        std::random_shuffle(legal_moves.begin(), legal_moves.end());

        std::string bestMove = "";
        int64_t bestResult = LLONG_MIN; 

        for(auto &move : legal_moves)
        {
            Engine temp_engine = engine;
            if(side == Side::left)
                temp_engine.make_move(move, "W");
            else
                temp_engine.make_move("W", move);

            auto result = search(temp_engine, other_side(side));

            if(result > bestResult) 
            {
                bestMove = move;
                bestResult = result;
            }
        }

        std::cerr << std::format("[minmax.cpp] my move: {0}\n", bestMove);
        return bestMove;
    }
};

int main()
{
    MinMaxBot bot(Eval_1{});
    bot.run();
}
