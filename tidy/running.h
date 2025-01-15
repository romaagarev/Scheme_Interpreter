#pragma once
#include "object.h"

class Function : public Object {
public:
    virtual ~Function() = default;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments) {
        return nullptr;
    };
};

std::shared_ptr<Object> FunctionMaker(std::string name);

std::shared_ptr<Object> Evaluate(std::shared_ptr<Object> arguments);

std::shared_ptr<Object> Running(std::shared_ptr<Object> func, std::shared_ptr<Object> arguments);