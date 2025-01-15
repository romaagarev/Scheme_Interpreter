#pragma once
#include <unordered_map>
#include <string>
#include "object.h"
#include "running.h"
#include "scheme.h"

void AddVariable(std::shared_ptr<Object> key, std::shared_ptr<Object> value);

void ReplaceVariable(std::shared_ptr<Object> key, std::shared_ptr<Object> value);

void RemoveVariable(std::shared_ptr<Object> key);

void ReplaceCar(std::shared_ptr<Object> key, std::shared_ptr<Object> value);

void ReplaceCdr(std::shared_ptr<Object> key, std::shared_ptr<Object> value);

void EraseKey(std::shared_ptr<Object> key);

bool Contains(std::shared_ptr<Object> key);

bool Contains(std::string key);

std::shared_ptr<Object> Get(std::shared_ptr<Object> key);

class SymbolEvaluation : public Function {
public:
    SymbolEvaluation() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class SymbolPredicate : public Function {
public:
    SymbolPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Define : public Function {
public:
    Define() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Set : public Function {
public:
    Set() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class SetCar : public Function {
public:
    SetCar() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class SetCdr : public Function {
public:
    SetCdr() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};