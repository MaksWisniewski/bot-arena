#pragma once

#include "../engine/engine.hpp"
#include "../json.hpp"

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

class BetterEval : public Eval
{
public:
    BetterEval() = default;
    BetterEval(const Json&);
    Type operator() (const Engine&, Side) const override;

private:
    Type income_weight = 1;
    Type control_weight = 1;
    Type turrets_weight = 1;
};
