#pragma once

#include "../engine/engine.hpp"

// klasa abstrakcyjna
class Eval
{
public:
    using Type = int64_t;
    virtual Type operator() (const Engine&, Side) const = 0;
};

class Eval_1 : public Eval
{
public:
    Type operator() (const Engine&, Side) const override;
};

class Eval_2 : public Eval
{
public:
    Type operator() (const Engine&, Side) const override;
};
