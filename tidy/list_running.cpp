#include "list_running.h"

std::shared_ptr<Object> PairPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        return (new Boolean(false))->GetRef();
    }
    if (As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(false))->GetRef();
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    if (!Is<Cell>(arguments)) {
        return (new Boolean(false))->GetRef();
    }
    auto cell = As<Cell>(arguments);
    if (!Is<Cell>(cell->GetFirst())) {
        if (!Is<Cell>(cell->GetSecond())) {
            return (new Boolean(true))->GetRef();
        }
        if (!As<Cell>(cell->GetSecond())->GetSecond() &&
            !Is<Cell>(As<Cell>(cell->GetSecond())->GetFirst())) {
            return (new Boolean(true))->GetRef();
        }
    }
    return (new Boolean(false))->GetRef();
}

std::shared_ptr<Object> NullPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    if (As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(false))->GetRef();
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    if (arguments) {
        return (new Boolean(false))->GetRef();
    }
    return (new Boolean(true))->GetRef();
}

std::shared_ptr<Object> ListPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    if (As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(false))->GetRef();
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    if (!arguments) {
        return (new Boolean(true))->GetRef();
    }
    if (!Is<Cell>(arguments)) {
        return (new Boolean(false))->GetRef();
    }
    auto cell = As<Cell>(arguments);
    if (!Is<Cell>(cell->GetSecond())) {
        if (cell->GetSecond()) {
            return (new Boolean(false))->GetRef();
        }
        return (new Boolean(true))->GetRef();
    }
    return (new Boolean(EasyListPredicate(cell->GetSecond())))->GetRef();
}

bool EasyListPredicate(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto cell = As<Cell>(arguments);
    if (!Is<Cell>(cell->GetSecond())) {
        if (cell->GetSecond()) {
            return false;
        }
        return true;
    }
    return EasyListPredicate(cell->GetSecond());
}

std::shared_ptr<Object> Cons::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto cell = As<Cell>(arguments);
    if (!Is<Cell>(cell->GetSecond()) || As<Cell>(cell->GetSecond())->GetSecond()) {
        throw RuntimeError("");
    }
    return (new Cell(Evaluate(cell->GetFirst()), Evaluate(As<Cell>(cell->GetSecond())->GetFirst()),
                     true))
        ->GetRef();
}

std::shared_ptr<Object> Cdr::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    if (As<Boolean>(As<Function>((new NullPredicate())->GetRef())->Eval(arguments))->GetValue()) {
        throw RuntimeError("");
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    return As<Cell>(arguments)->GetSecond();
}

std::shared_ptr<Object> Car::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    if (As<Boolean>(As<Function>((new NullPredicate())->GetRef())->Eval(arguments))->GetValue()) {
        throw RuntimeError("");
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    return As<Cell>(arguments)->GetFirst();
}

std::shared_ptr<Object> ListCreate::Eval(std::shared_ptr<Object> arguments) {
    return arguments;
}

std::shared_ptr<Object> ListRef::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    // As<Cell>(arguments)->SetFirst(Evaluate(As<Cell>(arguments)->GetFirst()));
    arguments = As<Function>((new Cons())->GetRef())->Eval(arguments);
    auto pos = As<Cell>(arguments)->GetSecond();
    if (!Is<Number>(pos)) {
        throw RuntimeError("");
    }
    int position = As<Number>(pos)->GetValue();
    auto array = As<Cell>(arguments)->GetFirst();
    while (position >= 0) {
        if (!array) {
            throw RuntimeError("");
        }
        if (position == 0) {
            return As<Cell>(array)->GetFirst();
        }
        array = As<Cell>(array)->GetSecond();
        position--;
    }
    throw RuntimeError("");
}

std::shared_ptr<Object> ListTail::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    // As<Cell>(arguments)->SetFirst(Evaluate(As<Cell>(arguments)->GetFirst()));
    arguments = As<Function>((new Cons())->GetRef())->Eval(arguments);
    auto pos = As<Cell>(arguments)->GetSecond();
    if (!Is<Number>(pos)) {
        throw RuntimeError("");
    }
    int position = As<Number>(pos)->GetValue();
    auto array = As<Cell>(arguments)->GetFirst();
    while (position >= 0) {
        if (position == 0) {
            return array;
        }
        if (!array) {
            throw RuntimeError("");
        }
        array = As<Cell>(array)->GetSecond();
        position--;
    }
    throw RuntimeError("");
}
