#include "object.h"
#include <iostream>
Number::Number(ConstantToken& token) : value_(token.value){};
int Number::GetValue() const {
    return value_;
}
Number::Number(int val) : value_(val){};

Symbol::Symbol(SymbolToken& token) {
    name_.reserve(15);
    name_ = token.name;
};

const std::string& Symbol::GetName() const {
    return name_;
}

void Symbol::SetName(std::string name) {
    name_ = name;
};

Symbol::Symbol(std::string name) {
    name_.reserve(15);
    name_ = name;
}
Cell::Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
    first_ = first;
    second_ = second;
    AddDependant(first);
    AddDependant(second);
}

Cell::Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second, bool br) {
    AddDependant(first);
    AddDependant(second);
    first_ = first;
    second_ = second;
    dotted_ = br;
}

Cell::Cell(std::shared_ptr<Object> other) {
    if (!Is<Cell>(other)) {
        throw RuntimeError("");
    }
    auto to_first = As<Cell>(other)->GetFirst();
    if (Is<Cell>(to_first)) {
        first_ = (new Cell(to_first))->GetRef();
    }
    if (Is<Number>(to_first)) {
        first_ = to_first;
    }
    if (Is<Symbol>(to_first)) {
        first_ = (new Symbol(As<Symbol>(to_first)->GetName()))->GetRef();
    }
    auto to_second = As<Cell>(other)->GetSecond();
    if (Is<Cell>(to_second)) {
        second_ = (new Cell(to_second))->GetRef();
    }
    if (Is<Number>(to_second)) {
        second_ = to_second;
    }
    if (Is<Symbol>(to_second)) {
        second_ = (new Symbol(As<Symbol>(to_second)->GetName()))->GetRef();
    }
    AddDependant(first_);
    AddDependant(second_);
    dotted_ = As<Cell>(other)->dotted_;
}

bool Cell::IsDotted() {
    return dotted_;
}
std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}
std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

void Cell::SetFirst(std::shared_ptr<Object> val) {
    DeleteDependant(first_);
    AddDependant(val);
    first_ = val;
}

void Cell::SetSecond(std::shared_ptr<Object> val) {
    DeleteDependant(second_);
    AddDependant(val);
    second_ = val;
}

Boolean::Boolean(BooleanToken& token) {
    value_ = token.value;
}

Boolean::Boolean(bool val) : value_(val){};

bool Boolean::GetValue() const {
    return value_;
}