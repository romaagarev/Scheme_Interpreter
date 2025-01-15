#include "variables.h"
#include "lambda.h"
#include <unordered_set>

std::unordered_map<std::string, std::shared_ptr<Object>> variables;
std::unordered_map<std::string, std::shared_ptr<LambdaEvaluating>> lambdas;
std::unordered_map<std::string, std::shared_ptr<Object>> local;
int call_count;

// std::unordered_map<std::string, std::shared_ptr<Object>> variable_pairs;

void LambdaEvaluating::ResetNames() {
    names_ = nullptr;
}
void LambdaEvaluating::ResetExpression() {
    expression_ = nullptr;
}
void AddVariable(std::shared_ptr<Object> key, std::shared_ptr<Object> value) {
    variables[As<Symbol>(key)->GetName()] = value;
}

void ReplaceVariable(std::shared_ptr<Object> key, std::shared_ptr<Object> value) {
    if (!Is<Number>(value) && !Is<Cell>(value)) {
        throw SyntaxError("");
    }
    if (!variables.contains(As<Symbol>(key)->GetName())) {
        throw NameError("");
    }
    variables[As<Symbol>(key)->GetName()] = value;
}

void RemoveVariable(std::shared_ptr<Object> key) {
    if (Contains(key)) {
        variables.erase(As<Symbol>(key)->GetName());
    }
}

void ReplaceCar(std::shared_ptr<Object> key, std::shared_ptr<Object> value) {
    if (!Is<Number>(value) && !Is<Cell>(value)) {
        throw SyntaxError("");
    }
    if (!variables.contains(As<Symbol>(key)->GetName())) {
        throw NameError("");
    }
    auto& pos = variables[As<Symbol>(key)->GetName()];
    if (!Is<Cell>(pos)) {
        throw SyntaxError("");
    }
    As<Cell>(pos)->SetFirst(value);
}

void ReplaceCdr(std::shared_ptr<Object> key, std::shared_ptr<Object> value) {
    if (!Is<Number>(value) && !Is<Cell>(value)) {
        throw SyntaxError("");
    }
    if (!variables.contains(As<Symbol>(key)->GetName())) {
        throw NameError("");
    }
    auto& pos = variables[As<Symbol>(key)->GetName()];
    if (!Is<Cell>(pos)) {
        throw SyntaxError("");
    }
    As<Cell>(pos)->SetSecond(value);
}

void EraseKey(std::shared_ptr<Object> key) {
    if (variables.contains(As<Symbol>(key)->GetName())) {
        variables.erase(As<Symbol>(key)->GetName());
    }
}

bool Contains(std::shared_ptr<Object> key) {
    return variables.contains(As<Symbol>(key)->GetName());
}

bool Contains(std::string key) {
    return variables.contains(key);
}

std::shared_ptr<Object> Get(std::shared_ptr<Object> key) {
    if (!Is<Symbol>(key)) {
        throw RuntimeError("");
    }
    if (!Contains(key)) {
        throw NameError("");
    }
    return variables[As<Symbol>(key)->GetName()];
}

std::shared_ptr<Object> Get(std::string key) {
    if (!Contains(key)) {
        throw NameError("");
    }
    return variables[key];
}

std::shared_ptr<Object> SymbolEvaluation::Eval(std::shared_ptr<Object> arguments) {
    if (Is<Symbol>(arguments)) {
        return Get(arguments);
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    if (As<Cell>(arguments)->GetSecond()) {
        throw RuntimeError("");
    }
    arguments = As<Cell>(arguments)->GetFirst();
    return Get(arguments);
}

std::shared_ptr<Object> SymbolPredicate::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        return (new Boolean(false))->GetRef();
    }
    if (As<Cell>(arguments)->GetSecond()) {
        return (new Boolean(false))->GetRef();
    }
    arguments = As<Cell>(arguments)->GetFirst();
    arguments = Evaluate(arguments);
    return (new Boolean(Is<Symbol>(arguments)))->GetRef();
}

std::shared_ptr<Object> SugarDefinition(std::shared_ptr<Object> arguments) {
    auto body = As<Cell>(arguments)->GetSecond();
    auto front = As<Cell>(arguments)->GetFirst();
    if (!Is<Cell>(front)) {
        throw SyntaxError("");
    }
    auto vars = As<Cell>(front)->GetSecond();
    auto name = As<Cell>(front)->GetFirst();
    if (!Is<Symbol>(name)) {
        throw SyntaxError("");
    }
    auto lld = (new Symbol("lambda"))->GetRef();
    auto back = (new Cell(vars, body))->GetRef();
    auto context = (new Cell(lld, back))->GetRef();
    auto pre_to_put = (new Cell(context, nullptr))->GetRef();
    auto to_put = (new Cell(name, pre_to_put))->GetRef();
    return As<Function>((new Define())->GetRef())->Eval(to_put);
}

std::shared_ptr<Object> Define::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    if (!Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
        return SugarDefinition(arguments);
    }
    auto name = As<Cell>(arguments)->GetFirst();
    std::shared_ptr<Object> value;
    if (Is<Cell>(As<Cell>(arguments)->GetSecond())) {
        auto cell = As<Cell>(As<Cell>(arguments)->GetSecond());
        if (cell->GetSecond()) {
            throw SyntaxError("");
        }
        auto val = cell->GetFirst();
        if (Is<Cell>(val) && Is<Symbol>(As<Cell>(val)->GetFirst()) &&
            (As<Symbol>(As<Cell>(val)->GetFirst())->GetName() == "lambda" ||
             LambdaContains(As<Cell>(val)->GetFirst()))) {
            value = Evaluate(val);
            if (!Is<Symbol>(value)) {
                AddVariable(name, value);
                return nullptr;
            }
            auto lam = LambdaFind(value);
            AddLambda(name, lam);
            EraseLambda(value);
            return nullptr;
        } else {
            value = Evaluate(cell->GetFirst());
        }

    } else {
        value = Evaluate(As<Cell>(arguments)->GetSecond());
    }
    if (std::dynamic_pointer_cast<RedefinedOperator>(value)) {
        As<Symbol>(name)->SetName(As<Symbol>(name)->GetName());
        AddLambda(name, std::dynamic_pointer_cast<RedefinedOperator>(value));
    } else {
        AddVariable(name, value);
    }
    return nullptr;
}

std::shared_ptr<Object> Set::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    if (!Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
        throw SyntaxError("");
    }
    auto name = As<Cell>(arguments)->GetFirst();
    std::shared_ptr<Object> value;
    if (Is<Cell>(As<Cell>(arguments)->GetSecond())) {
        auto cell = As<Cell>(As<Cell>(arguments)->GetSecond());
        if (cell->GetSecond()) {
            throw SyntaxError("");
        }
        value = Evaluate(cell->GetFirst());
    } else {
        value = Evaluate(As<Cell>(arguments)->GetSecond());
    }

    ReplaceVariable(name, value);
    return nullptr;
}

std::shared_ptr<Object> SetCar::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    //    if (!Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
    //        throw SyntaxError("");
    //    }
    auto name = As<Cell>(arguments)->GetFirst();
    std::shared_ptr<Object> value;
    if (Is<Cell>(As<Cell>(arguments)->GetSecond())) {
        auto cell = As<Cell>(As<Cell>(arguments)->GetSecond());
        if (cell->GetSecond()) {
            throw SyntaxError("");
        }
        value = Evaluate(cell->GetFirst());
    } else {
        value = Evaluate(As<Cell>(arguments)->GetSecond());
    }
    auto to = Evaluate(name);
    if (!Is<Cell>(to)) {
        throw SyntaxError("");
    }
    As<Cell>(to)->SetFirst(value);
    return nullptr;
}

std::shared_ptr<Object> SetCdr::Eval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    //    if (!Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
    //        throw SyntaxError("");
    //    }
    auto name = As<Cell>(arguments)->GetFirst();
    std::shared_ptr<Object> value;
    if (Is<Cell>(As<Cell>(arguments)->GetSecond())) {
        auto cell = As<Cell>(As<Cell>(arguments)->GetSecond());
        if (cell->GetSecond()) {
            throw SyntaxError("");
        }
        value = Evaluate(cell->GetFirst());
    } else {
        value = Evaluate(As<Cell>(arguments)->GetSecond());
    }
    auto to = Evaluate(name);
    if (!Is<Cell>(to)) {
        throw SyntaxError("");
    }
    As<Cell>(to)->SetSecond(value);
    return nullptr;
}

RedefinedOperator::RedefinedOperator(std::shared_ptr<Object> func)
    : my_func_(std::dynamic_pointer_cast<Function>(func)) {
    AddDependant(my_func_);
};

bool LambdaContains(std::shared_ptr<Object> key) {
    if (!Is<Symbol>(key)) {
        throw SyntaxError("");
    }
    return lambdas.contains(As<Symbol>(key)->GetName());
}

bool LambdaContains(std::string key) {
    return lambdas.contains(key);
}

void AddLambda(std::shared_ptr<Object> key, std::shared_ptr<LambdaEvaluating> value) {
    lambdas[As<Symbol>(key)->GetName()] = value;
    // local.back().insert(As<Symbol>(key)->GetName());
}

void AddLambdaBB(std::shared_ptr<Object> key, std::shared_ptr<LambdaEvaluating> value) {
    lambdas[As<Symbol>(key)->GetName()] = value;
    // local[local.size() - 2].insert(As<Symbol>(key)->GetName());
}

void EraseLambda(std::shared_ptr<Object> key) {
    if (!LambdaContains(key)) {
        throw NameError("");
    }
    lambdas.erase(As<Symbol>(key)->GetName());
}

std::shared_ptr<LambdaEvaluating> LambdaFind(std::shared_ptr<Object> key) {
    if (!LambdaContains(key)) {
        throw NameError("");
    }
    return lambdas[As<Symbol>(key)->GetName()];
}

std::shared_ptr<LambdaEvaluating> LambdaFind(std::string key) {
    if (!LambdaContains(key)) {
        throw NameError("");
    }
    return lambdas[key];
}

int LambdaEvaluating::GetAlives() {
    return call_count;
}

void LambdaEvaluating::IncAlives() {
    // call_count++;
}

void LambdaEvaluating::DecAlives() {
    // call_count--;
}

int LambdaEvaluating::Num() {
    return my_num_;
}

int Lambda::Num() {
    return my_num_;
}

void Lambda::SetNum(int num) {
    my_num_ = num;
}

LambdaEvaluating::LambdaEvaluating(int num, std::shared_ptr<Object> names,
                                   std::shared_ptr<Object> expression) {
    names_ = names;
    expression_ = expression;
    my_num_ = num;
    AddDependant(names);
    AddDependant(expression);
    IncAlives();
}

LambdaEvaluating::~LambdaEvaluating() {
    DecAlives();
};

void ReplaceContext(std::shared_ptr<Object> arguments, int num) {
    if (Is<Cell>(arguments)) {
        if (Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
            auto prev_name = As<Symbol>(As<Cell>(arguments)->GetFirst())->GetName();
            auto ind = prev_name.find_last_of('_');
            ind += 3;
            if (local.contains(prev_name.substr(0, ind) + std::to_string(num))) {
                As<Cell>(arguments)->SetFirst(
                    local[prev_name.substr(0, ind) + std::to_string(num)]);
            }
        }
        if (Is<Symbol>(As<Cell>(arguments)->GetSecond())) {
            auto prev_name = As<Symbol>(As<Cell>(arguments)->GetFirst())->GetName();
            auto ind = prev_name.find_last_of('_');
            ind += 3;
            if (local.contains(prev_name.substr(0, ind) + std::to_string(num))) {
                As<Cell>(arguments)->SetSecond(
                    local[prev_name.substr(0, ind) + std::to_string(num)]);
            }
        }
        ReplaceContext(As<Cell>(arguments)->GetFirst(), num);
        ReplaceContext(As<Cell>(arguments)->GetSecond(), num);
    }
}

void ReplaceContextStart(std::shared_ptr<Object> arguments, int num) {
    //    if (Is<Symbol>(arguments)) {
    //        auto& prev_name = As<Symbol>(arguments)->GetName();
    //        if (local.contains(prev_name + "_xx" + std::to_string(num))) {
    //            As<Symbol>(arguments)->SetName(prev_name + "_xx" + std::to_string(num));
    //        }
    //    }
    if (Is<Cell>(arguments)) {
        if (Is<Symbol>(As<Cell>(arguments)->GetFirst())) {
            auto& prev_name = As<Symbol>(As<Cell>(arguments)->GetFirst())->GetName();
            if (local.contains(prev_name + "_xx" + std::to_string(num))) {
                As<Cell>(arguments)->SetFirst(local[prev_name + "_xx" + std::to_string(num)]);
            }
        }
        if (Is<Symbol>(As<Cell>(arguments)->GetSecond())) {
            auto& prev_name = As<Symbol>(As<Cell>(arguments)->GetSecond())->GetName();
            if (local.contains(prev_name + "_xx" + std::to_string(num))) {
                As<Cell>(arguments)->SetSecond(local[prev_name + "_xx" + std::to_string(num)]);
            }
        }
        ReplaceContextStart(As<Cell>(arguments)->GetFirst(), num);
        ReplaceContextStart(As<Cell>(arguments)->GetSecond(), num);
    }
}

void AddContext(std::shared_ptr<Object> arguments, int num) {
    if (!arguments) {
        return;
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto cell = As<Cell>(arguments);
    AddContext(cell->GetSecond(), num);
    if (!Is<Symbol>(cell->GetFirst())) {
        throw RuntimeError("");
    }
    auto prev_name = As<Symbol>(cell->GetFirst())->GetName();
    auto ind = prev_name.find_last_of('_');
    ind += 3;
    As<Symbol>(cell->GetFirst())->SetName(prev_name.substr(0, ind) + std::to_string(num));
    // AddVariable(cell->GetFirst(), nullptr);
    local[As<Symbol>(cell->GetFirst())->GetName()] = As<Symbol>(cell->GetFirst());
}

void AddContextStart(std::shared_ptr<Object> arguments, int num) {
    if (!arguments) {
        return;
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto cell = As<Cell>(arguments);
    AddContextStart(cell->GetSecond(), num);
    if (!Is<Symbol>(cell->GetFirst())) {
        throw RuntimeError("");
    }
    auto prev_name = As<Symbol>(cell->GetFirst())->GetName();
    As<Symbol>(cell->GetFirst())->SetName(prev_name + "_xx" + std::to_string(num));
    local[As<Symbol>(cell->GetFirst())->GetName()] = As<Symbol>(cell->GetFirst());
}

void RemoveContext(std::shared_ptr<Object> arguments) {
    if (!arguments) {
        return;
    }
    if (!Is<Cell>(arguments)) {
        throw RuntimeError("");
    }
    auto cell = As<Cell>(arguments);
    RemoveContext(cell->GetSecond());
    if (!Is<Symbol>(cell->GetFirst())) {
        throw RuntimeError("");
    }
    RemoveVariable(cell->GetFirst());
}

std::shared_ptr<Object> Lambda::Eval(std::shared_ptr<Object> arguments) {
    SetNum(call_count);
    call_count++;
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    auto cell = As<Cell>(arguments);
    if (cell->GetFirst() && !Is<Cell>(cell->GetFirst())) {
        throw SyntaxError("");
    }
    if (!cell->GetSecond()) {
        throw SyntaxError("");
    }
    AddContextStart(cell->GetFirst(), call_count);
    ReplaceContextStart(cell->GetSecond(), call_count);
    local.clear();
    auto name = (new Symbol("_tt" + std::to_string(Num())))->GetRef();
    call_count++;
    auto my_lambda_evaluating =
        (new LambdaEvaluating(Num(), cell->GetFirst(), cell->GetSecond()))->GetRef();
    AddLambda(name, std::dynamic_pointer_cast<LambdaEvaluating>(my_lambda_evaluating));
    return name;
}

void LambdaEvaluating::SetUpNames(std::shared_ptr<Object> names,
                                  std::shared_ptr<Object> arguments) {
    if (!names && !arguments) {
        return;
    }
    if (!Is<Cell>(arguments) || !Is<Cell>(names)) {
        throw RuntimeError("");
    }
    auto first = As<Cell>(names)->GetFirst();
    auto first_name = As<Cell>(arguments)->GetFirst();
    first_name = Evaluate(first_name);
    AddVariable(first, first_name);
    SetUpNames(As<Cell>(names)->GetSecond(), As<Cell>(arguments)->GetSecond());
}

std::shared_ptr<Object> SimpleEval(std::shared_ptr<Object> arguments) {
    if (!Is<Cell>(arguments)) {
        throw SyntaxError("");
    }
    auto cell = As<Cell>(arguments);
    if (!cell->GetSecond()) {
        if (Is<Symbol>(cell->GetFirst()) && LambdaContains(cell->GetFirst())) {
            return cell->GetFirst();
        } else {
            return Evaluate(cell->GetFirst());
        }
    }
    Evaluate(cell->GetFirst());
    return SimpleEval(cell->GetSecond());
}

void DefineChecking(std::shared_ptr<Object> arguments, int num) {
    if (Is<Symbol>(arguments)) {
        auto prev_name = As<Symbol>(arguments)->GetName();
        if (local.contains(prev_name + "_tt" + std::to_string(num))) {
            As<Symbol>(arguments)->SetName(prev_name + "_tt" + std::to_string(num));
        }
    }
    if (Is<Cell>(arguments)) {
        auto cell = As<Cell>(arguments);
        if (Is<Symbol>(cell->GetFirst()) && As<Symbol>(cell->GetFirst())->GetName() == "define") {
            auto to = cell->GetSecond();
            if (!Is<Cell>(to)) {
                throw SyntaxError("aaa");
            }
            auto r = As<Cell>(to);
            if (Is<Symbol>(r->GetFirst())) {
                auto prev_name = As<Symbol>(r->GetFirst())->GetName();
                As<Symbol>(r->GetFirst())->SetName(prev_name + "_tt" + std::to_string(num));
                local[As<Symbol>(r->GetFirst())->GetName()] = r->GetFirst();
            }
            if (Is<Cell>(r->GetFirst())) {
                auto tt = As<Cell>(r->GetFirst());
                if (!Is<Symbol>(tt->GetFirst())) {
                    throw SyntaxError("bbb");
                }
                auto prev_name = As<Symbol>(tt->GetFirst())->GetName();
                As<Symbol>(tt->GetFirst())->SetName(prev_name + "_tt" + std::to_string(num));
                local[As<Symbol>(tt->GetFirst())->GetName()] = tt->GetFirst();
            }
            DefineChecking(r->GetSecond(), num);
        } else {
            DefineChecking(As<Cell>(arguments)->GetFirst(), num);
            DefineChecking(As<Cell>(arguments)->GetSecond(), num);
        }
    }
}

LambdaEvaluating::LambdaEvaluating(std::shared_ptr<Object> other) {
    if (As<LambdaEvaluating>(other)->names_) {
        names_ = (new Cell(As<LambdaEvaluating>(other)->names_))->GetRef();
        // AddDependant(names_);
    }
    if (As<LambdaEvaluating>(other)->expression_) {
        expression_ = (new Cell(As<LambdaEvaluating>(other)->expression_))->GetRef();
        // AddDependant(expression_);
    }
    my_num_ = call_count;
}

std::shared_ptr<Object> LambdaEvaluating::FuncEval(std::shared_ptr<Object> arguments) {
    DefineChecking(expression_, call_count);
    local.clear();
    AddContext(names_, call_count);
    ReplaceContext(expression_, call_count);
    local.clear();
    call_count++;
    SetUpNames(names_, arguments);
    auto ret = SimpleEval(LambdaEvaluating::expression_);
    return ret;
}

std::shared_ptr<Object> RedefinedOperator::FuncEval(std::shared_ptr<Object> arguments) {
    return my_func_->Eval(arguments);
}

std::shared_ptr<Object> LambdaEvaluating::Eval(std::shared_ptr<Object> arguments) {
    auto ret = FuncEval(arguments);
    return ret;
}
