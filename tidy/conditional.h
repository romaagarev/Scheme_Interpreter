#pragma once
#include "running.h"

class IfConstruction : public Function {
public:
    IfConstruction() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};
