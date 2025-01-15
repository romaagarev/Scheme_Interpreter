#pragma once
#include "running.h"

class PairPredicate : public Function {
public:
    PairPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class NullPredicate : public Function {
public:
    NullPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class ListPredicate : public Function {
public:
    ListPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

bool EasyListPredicate(std::shared_ptr<Object> arguments);

class Cons : public Function {
public:
    Cons() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Cdr : public Function {
public:
    Cdr() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Car : public Function {
public:
    Car() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class ListCreate : public Function {
public:
    ListCreate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class ListRef : public Function {
public:
    ListRef() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class ListTail : public Function {
public:
    ListTail() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};
