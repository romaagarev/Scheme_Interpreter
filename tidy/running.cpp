#include "running.h"
#include "eval_running.h"
#include "integer_running.h"
#include "boolean_running.h"
#include "list_running.h"
#include "variables.h"
#include "conditional.h"
#include "lambda.h"
#include <iostream>

int t = 0;

std::shared_ptr<Object> FunctionMaker(std::string name) {
    if (Contains(name)) {
        auto func = ((new SymbolEvaluation())->GetRef());
        return func;
    }
    if (LambdaContains(name + "ro")) {
        auto func = (LambdaFind(name + "ro"));
        return func;
    }
    if (LambdaContains(name)) {
        auto func = (LambdaFind(name));
        return func;
    }
    if (name == "quote") {
        auto func = ((new QuoteFunction())->GetRef());
        return func;
    }
    if (name == "number?") {
        auto func = ((new NumberPredicate())->GetRef());
        return func;
    }
    if (name == ">") {
        auto func = ((new Great())->GetRef());
        return func;
    }
    if (name == "=") {
        auto func = ((new Equal())->GetRef());
        return func;
    }
    if (name == "<") {
        auto func = ((new Less())->GetRef());
        return func;
    }
    if (name == ">=") {
        auto func = ((new Geq())->GetRef());
        return func;
    }
    if (name == "<=") {
        auto func = ((new Leq())->GetRef());
        return func;
    }
    if (name == "+") {
        auto func = ((new Add())->GetRef());
        return func;
    }
    if (name == "-") {
        auto func = ((new Sub())->GetRef());
        return func;
    }
    if (name == "*") {
        auto func = ((new Mul())->GetRef());
        return func;
    }
    if (name == "/") {
        auto func = ((new Divide())->GetRef());
        return func;
    }
    if (name == "max") {
        auto func = ((new Max())->GetRef());
        return func;
    }
    if (name == "min") {
        auto func = ((new Min())->GetRef());
        return func;
    }
    if (name == "abs") {
        auto func = ((new Abs())->GetRef());
        return func;
    }
    if (name == "boolean?") {
        auto func = ((new BooleanPredicate())->GetRef());
        return func;
    }
    if (name == "not") {
        auto func = ((new Not())->GetRef());
        return func;
    }
    if (name == "and") {
        auto func = ((new And())->GetRef());
        return func;
    }
    if (name == "or") {
        auto func = ((new Or())->GetRef());
        return func;
    }
    if (name == "pair?") {
        auto func = ((new PairPredicate())->GetRef());
        return func;
    }
    if (name == "null?") {
        auto func = ((new NullPredicate())->GetRef());
        return func;
    }
    if (name == "list?") {
        auto func = ((new ListPredicate())->GetRef());
        return func;
    }
    if (name == "cdr") {
        auto func = ((new Cdr())->GetRef());
        return func;
    }
    if (name == "car") {
        auto func = ((new Car())->GetRef());
        return func;
    }
    if (name == "cons") {
        auto func = ((new Cons())->GetRef());
        return func;
    }
    if (name == "list") {
        auto func = ((new ListCreate())->GetRef());
        return func;
    }
    if (name == "list-ref") {
        auto func = ((new ListRef())->GetRef());
        return func;
    }
    if (name == "list-tail") {
        auto func = ((new ListTail())->GetRef());
        return func;
    }
    if (name == "symbol?") {
        auto func = ((new SymbolPredicate())->GetRef());
        return func;
    }
    if (name == "define") {
        auto func = ((new Define())->GetRef());
        return func;
    }
    if (name == "set!") {
        auto func = ((new Set())->GetRef());
        return func;
    }
    if (name == "if") {
        auto func = ((new IfConstruction())->GetRef());
        return func;
    }
    if (name == "set-car!") {
        auto func = ((new SetCar())->GetRef());
        return func;
    }
    if (name == "set-cdr!") {
        auto func = ((new SetCdr())->GetRef());
        return func;
    }
    if (name == "lambda") {
        auto func = ((new Lambda())->GetRef());
        return func;
    }
    throw NameError("");
}

std::shared_ptr<Object> Evaluate(std::shared_ptr<Object> arguments) {
    if (Is<Number>(arguments)) {
        return arguments;
    }
    if (Is<Boolean>(arguments)) {
        return arguments;
    }
    if (Is<Symbol>(arguments)) {

        auto oper_name = As<Symbol>(arguments)->GetName();
        std::shared_ptr<Object> func(FunctionMaker(oper_name));
        if (!std::dynamic_pointer_cast<SymbolEvaluation>(func)) {
            return (new RedefinedOperator(func))->GetRef();
        }
        auto ret = Running(func, arguments);
        return ret;
    }
    if (!arguments) {
        throw RuntimeError("");
    }
    if (Is<Cell>(arguments)) {
        auto cell = As<Cell>(arguments);
        auto oper = As<Cell>(arguments)->GetFirst();
        if (Is<Cell>(oper)) {
            auto ret = Evaluate(oper);
            oper.reset();
            oper = ret;
        }
        if (!Is<Symbol>(oper)) {
            throw RuntimeError("");
        }
        // oper = Evaluate(oper);
        auto oper_name = As<Symbol>(oper)->GetName();
        //        std::cout << oper;
        std::shared_ptr<Object> func = FunctionMaker(oper_name);
        if (Is<LambdaEvaluating>(func) && !Is<RedefinedOperator>(func)) {
            func = (new LambdaEvaluating(func))->GetRef();
        }
        auto ret = Running(func, As<Cell>(arguments)->GetSecond());
        return ret;
    }
    throw RuntimeError("");
}

std::shared_ptr<Object> Running(std::shared_ptr<Object> func, std::shared_ptr<Object> arguments) {
    if (std::dynamic_pointer_cast<LambdaEvaluating>(func)) {
        std::shared_ptr<LambdaEvaluating> ff = std::dynamic_pointer_cast<LambdaEvaluating>(func);
        return ff->Eval(arguments);
    }
    return std::dynamic_pointer_cast<Function>(func)->Eval(arguments);
}
