#include "eval_running.h"

std::shared_ptr<Object> QuoteFunction::Eval(std::shared_ptr<Object> arguments) {
    if (Is<Cell>(arguments) && !As<Cell>(arguments)->GetSecond()) {
        return As<Cell>(arguments)->GetFirst();
    }
    return arguments;
}
