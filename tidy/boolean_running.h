#pragma once
#include "running.h"

class BooleanPredicate : public Function {
public:
    BooleanPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Not : public Function {
public:
    Not() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class And : public Function {
public:
    And() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Or : public Function {
public:
    Or() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};