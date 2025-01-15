#include "integer_running.h"

std::shared_ptr<Object> NumberPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (Is<Cell>(arguments) && !As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(Is<Number>(As<Cell>(arguments)->GetFirst())))->GetRef();
    }
    return (new Boolean(Is<Number>(arguments)))->GetRef();
}

std::shared_ptr<Object> Great::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    auto to_comp = forward;
    if (Is<Cell>(forward)) {
        to_comp = As<Cell>(forward)->GetFirst();
    } else {
        if (!Is<Number>(forward)) {
            if (Is<Number>(current)) {
                return (new Boolean(true))->GetRef();
            }
            throw RuntimeError("");
        }
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    if (!to_comp) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Number>(to_comp)) {
        throw RuntimeError("");
    }
    if (As<Number>(current)->GetValue() <= As<Number>(to_comp)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    return Eval(forward);
}

std::shared_ptr<Object> Less::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    auto to_comp = forward;
    if (Is<Cell>(forward)) {
        to_comp = As<Cell>(forward)->GetFirst();
    } else {
        if (!Is<Number>(forward)) {
            if (Is<Number>(current)) {
                return (new Boolean(true))->GetRef();
            }
            throw RuntimeError("");
        }
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    if (!to_comp) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Number>(to_comp)) {
        throw RuntimeError("");
    }
    if (As<Number>(current)->GetValue() >= As<Number>(to_comp)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    return Eval(forward);
}

std::shared_ptr<Object> Equal::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    auto to_comp = forward;
    if (Is<Cell>(forward)) {
        to_comp = As<Cell>(forward)->GetFirst();
    } else {
        if (!Is<Number>(forward)) {
            if (Is<Number>(current)) {
                return (new Boolean(true))->GetRef();
            }
            throw RuntimeError("");
        }
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    if (!to_comp) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Number>(to_comp)) {
        throw RuntimeError("");
    }
    if (As<Number>(current)->GetValue() != As<Number>(to_comp)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    return Eval(forward);
}

std::shared_ptr<Object> Geq::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    auto to_comp = forward;
    if (Is<Cell>(forward)) {
        to_comp = As<Cell>(forward)->GetFirst();
    } else {
        if (!Is<Number>(forward)) {
            if (Is<Number>(current)) {
                return (new Boolean(true))->GetRef();
            }
            throw RuntimeError("");
        }
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    if (!to_comp) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Number>(to_comp)) {
        throw RuntimeError("");
    }
    if (As<Number>(current)->GetValue() < As<Number>(to_comp)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    return Eval(forward);
}

std::shared_ptr<Object> Leq::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    auto to_comp = forward;
    if (Is<Cell>(forward)) {
        to_comp = As<Cell>(forward)->GetFirst();
    } else {
        if (!Is<Number>(forward)) {
            if (Is<Number>(current)) {
                return (new Boolean(true))->GetRef();
            }
            throw RuntimeError("");
        }
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    if (!to_comp) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Number>(to_comp)) {
        throw RuntimeError("");
    }
    if (As<Number>(current)->GetValue() > As<Number>(to_comp)->GetValue()) {
        return (new Boolean(false))->GetRef();
    }
    return Eval(forward);
}

std::shared_ptr<Object> Add::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Number(0))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(As<Number>(current)->GetValue() + As<Number>(forward)->GetValue()))
        ->GetRef();
}

std::shared_ptr<Object> Sub::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = As<Function>((new Add())->GetRef())->Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(As<Number>(current)->GetValue() - As<Number>(forward)->GetValue()))
        ->GetRef();
}

std::shared_ptr<Object> Mul::Eval(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return (new Number(1))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(As<Number>(current)->GetValue() * As<Number>(forward)->GetValue()))
        ->GetRef();
}

std::shared_ptr<Object> Divide::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = As<Function>((new Mul())->GetRef())->Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(As<Number>(current)->GetValue() / As<Number>(forward)->GetValue()))
        ->GetRef();
}

std::shared_ptr<Object> Max::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(std::max(As<Number>(current)->GetValue(), As<Number>(forward)->GetValue())))
        ->GetRef();
}

std::shared_ptr<Object> Min::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (!forward) {
        if (Is<Number>(current)) {
            return current;
        }
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    forward = Eval(forward);
    if (!Is<Number>(forward)) {
        throw RuntimeError("");
    }
    return (new Number(std::min(As<Number>(current)->GetValue(), As<Number>(forward)->GetValue())))
        ->GetRef();
}

std::shared_ptr<Object> Abs::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto forward = As<Cell>(arguments)->GetSecond();
    auto current = As<Cell>(arguments)->GetFirst();
    current = Evaluate(current);
    // auto to_comp = forward;
    if (forward) {
        throw RuntimeError("");
    }
    if (!Is<Number>(current)) {
        throw RuntimeError("");
    }
    return (new Number(abs(As<Number>(current)->GetValue())))->GetRef();
}
