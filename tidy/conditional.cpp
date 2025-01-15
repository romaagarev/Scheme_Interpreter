#include "conditional.h"

std::shared_ptr<Object> IfConstruction::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    auto condition = As<Cell>(arguments)->GetFirst();
    arguments = As<Cell>(arguments)->GetSecond();
    condition = Evaluate(condition);
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    if (!Is<Boolean>(condition)) {
        throw SyntaxError("");
    }
    if (As<Boolean>(condition)->GetValue()) {
        arguments = As<Cell>(arguments)->GetFirst();
        return Evaluate(arguments);
    } else {
        if (!As<Cell>(arguments)->GetSecond()) {
            return nullptr;
        } else {
            arguments = As<Cell>(arguments)->GetSecond();
            if (Is<Cell>(arguments)) {
                arguments = As<Cell>(arguments)->GetFirst();
            }
            return Evaluate(arguments);
        }
    }
}
