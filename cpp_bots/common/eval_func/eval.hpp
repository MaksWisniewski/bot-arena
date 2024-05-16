#pragma once


#include "../engine/engine.hpp"

// klasa abstrakcyjna
class Eval 
{
    public:
        using Type = int64_t;
        virtual Type operator() (Engine &engine, Side mySide) const;
};

class Eval_1 : public Eval
{
    public:
        Type operator() (Engine &engine, Side mySide) const override;
};