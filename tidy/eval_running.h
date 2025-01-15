#pragma once
#include "running.h"

class QuoteFunction : public Function {
public:
    QuoteFunction() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};
