#include "boolean_running.h"

std::shared_ptr<Object> BooleanPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (Is<Cell>(arguments) && !As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(Is<Boolean>(As<Cell>(arguments)->GetFirst())))->GetRef();
    }
    return (new Boolean(Is<Boolean>(arguments)))->GetRef();
}

std::shared_ptr<Object> Not::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        throw RuntimeError("");
    }
    if (Is<Cell>(arguments) && !As<Cell>(arguments)->GetSecond()) {
        arguments = As<Cell>(arguments)->GetFirst();
    }
    arguments = Evaluate(arguments);
    if (!Is<Boolean>(arguments)) {
        return (new Boolean(false))->GetRef();
    }
    return (new Boolean(not As<Boolean>(arguments)->GetValue()))->GetRef();
}

std::shared_ptr<Object> And::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (Is<Boolean>(current) && not As<Boolean>(current)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    if (!forward) {
        return current;
    }
    return Eval(forward);
}

std::shared_ptr<Object> Or::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(false))->GetRef();
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (Is<Boolean>(current) && As<Boolean>(current)->GetValue()) {
        return (new Boolean(true))->GetRef();
    }
    if (!forward) {
        return current;
    }
    return Eval(forward);
}