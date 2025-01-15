#pragma once
#include "running.h"

class NumberPredicate : public Function {
public:
    NumberPredicate() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Great : public Function {
public:
    Great() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Equal : public Function {
public:
    Equal() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Less : public Function {
public:
    Less() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Geq : public Function {
public:
    Geq() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Leq : public Function {
public:
    Leq() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Add : public Function {
public:
    Add() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Sub : public Function {
public:
    Sub() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Mul : public Function {
public:
    Mul() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Divide : public Function {
public:
    Divide() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Max : public Function {
public:
    Max() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Min : public Function {
public:
    Min() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};

class Abs : public Function {
public:
    Abs() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
};