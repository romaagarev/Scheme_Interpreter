#include "scheme.h"
#include "variables.cpp"
#include "heap.cpp"
#include <iostream>
#include <vector>

void FinalDelete() {
    for (auto cur : all_objects) {
        if (Is<Cell>(cur)) {
            As<Cell>(cur)->SetFirst(nullptr);
            As<Cell>(cur)->SetSecond(nullptr);
        }
        if (Is<LambdaEvaluating>(cur)) {
            As<LambdaEvaluating>(cur)->ResetExpression();
            As<LambdaEvaluating>(cur)->ResetNames();
        }
        cur->ResetAll();
    }
}

Interpreter::~Interpreter() {
    FinalDelete();
    variables.clear();
    lambdas.clear();
    local.clear();
    all_objects.clear();
}

std::string Print(std::shared_ptr<Object> result) {
    if (Is<Number>(result)) {
        return std::to_string(As<Number>(result)->GetValue());
    }
    if (Is<Cell>(result)) {
        if (As<Cell>(result)->GetFirst() == nullptr) {
            if (As<Cell>(result)->GetSecond() == nullptr) {
                return "()";
            }
            return Print(As<Cell>(result)->GetSecond());
        }
        if (As<Cell>(result)->GetSecond() == nullptr) {
            return Print(As<Cell>(result)->GetFirst());
        }
        if (As<Cell>(result)->IsDotted() && !Is<Cell>(As<Cell>(result)->GetSecond())) {
            return Print(As<Cell>(result)->GetFirst()) + " . " +
                   Print(As<Cell>(result)->GetSecond());
        }
        return Print(As<Cell>(result)->GetFirst()) + " " + Print(As<Cell>(result)->GetSecond());
    }
    if (Is<Symbol>(result)) {
        return As<Symbol>(result)->GetName();
    }
    if (Is<Boolean>(result)) {
        if (As<Boolean>(result)->GetValue()) {
            return "#t";
        } else {
            return "#f";
        }
    }
    return "";
}

auto ReadF(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};

    auto obj = Read(&tokenizer);
    return obj;
}

std::string Interpreter::Run(const std::string& string) {
    // std::cout << l << ' ';
    auto to_eval = ReadF(string);
    auto evaluated = Evaluate(to_eval);
    std::string ret;

    if (Is<Cell>(evaluated)) {
        ret = "(" + Print(evaluated) + ")";
    } else if (!evaluated) {
        ret = "()";
    } else {
        ret = Print(evaluated);
    }
    //    for(auto s : lambdas){
    //        std::cout << s.first << '\n';
    //    }
    StartMarks();
    local.clear();
    for (auto objects : all_objects) {
        SpecialDelete(objects);
    }
    std::unordered_set<std::shared_ptr<Object>> no_erase;
    for (auto objects : all_objects) {
        if (objects->IsMarked()) {
            no_erase.insert(objects);
        }
    }
    all_objects.clear();
    all_objects = no_erase;
    NewDelete();
    ResetMarks();
    // return "100";
    // std::cout << l << '\n';
    return ret;
}

void StartMarks() {
    for (auto vals : variables) {
        auto name = vals.first;
        auto ind = name.find('_');
        if (ind != std::string::npos) {
        } else {
            vals.second->Mark();
        }
    }
    for (auto vals : lambdas) {
        auto name = vals.first;
        auto ind = name.find('_');
        if (ind != std::string::npos) {
        } else {
            vals.second->Mark();
        }
    }
}

void SpecialDelete(std::shared_ptr<Object>& cur) {
    if (Is<Function>(cur)) {
        if (Is<LambdaEvaluating>(cur) && !cur->IsMarked()) {
            As<LambdaEvaluating>(cur)->ResetExpression();
            As<LambdaEvaluating>(cur)->ResetNames();
        }
        return;
    }
    if (Is<Cell>(cur) && !cur->IsMarked()) {
        As<Cell>(cur)->SetFirst(nullptr);
        As<Cell>(cur)->SetSecond(nullptr);
        cur->ResetAll();
    }
    if (Is<LambdaEvaluating>(cur) && !cur->IsMarked()) {
        As<LambdaEvaluating>(cur)->ResetExpression();
        As<LambdaEvaluating>(cur)->ResetNames();
        cur->ResetAll();
    }
}

void NewDelete() {
    std::vector<std::string> to_erase;
    for (auto vals : variables) {
        auto name = vals.first;
        if (vals.second->IsMarked()) {
            continue;
        }
        auto ind = name.find_last_of('_');
        if (ind != std::string::npos) {
            to_erase.push_back(name);
        }
    }
    for (auto names : to_erase) {
        variables.erase(names);
    }
    to_erase.clear();
    for (auto vals : lambdas) {
        auto name = vals.first;
        if (vals.second->IsMarked()) {
            continue;
        }
        auto ind = name.find_last_of('_');
        if (ind != std::string::npos) {
            to_erase.push_back(name);
        }
    }
    for (auto names : to_erase) {
        lambdas.erase(names);
    }
}

void ResetMarks() {
    for (auto obj : all_objects) {
        if (obj) {
            obj->ResetMark();
        }
    }
}
