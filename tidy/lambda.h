#pragma once
#include "running.h"
#include "variables.h"

class Lambda : public Function {
public:
    Lambda() = default;
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments);
    int Num();
    void SetNum(int num);

private:
    int my_num_;
};

class LambdaEvaluating : public Function {
public:
    LambdaEvaluating(int num, std::shared_ptr<Object> names, std::shared_ptr<Object> expression);
    LambdaEvaluating(std::shared_ptr<Object> other);
    std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments) override;
    static void IncAlives();
    static void DecAlives();
    LambdaEvaluating() = default;
    virtual std::shared_ptr<Object> FuncEval(std::shared_ptr<Object> arguments);
    int Num();
    static int GetAlives();
    ~LambdaEvaluating() override;
    void SetUpNames(std::shared_ptr<Object> names, std::shared_ptr<Object> arguments);
    void ResetNames();
    void ResetExpression();

protected:
    int my_num_;
    std::shared_ptr<Object> names_;
    std::shared_ptr<Object> expression_;
};

class RedefinedOperator : public LambdaEvaluating {
public:
    RedefinedOperator(std::shared_ptr<Object> func);
    std::shared_ptr<Object> FuncEval(std::shared_ptr<Object> arguments) override;

private:
    std::shared_ptr<Function> my_func_;
};

std::shared_ptr<Object> SimpleEval(std::shared_ptr<Object>);

void ReplaceContext(std::shared_ptr<Object> arguments, int num);

void AddContext(std::shared_ptr<Object> arguments, int num);

bool LambdaContains(std::shared_ptr<Object> key);

bool LambdaContains(std::string key);

void DefineChecking(std::shared_ptr<Object> arguments, int num);

void AddLambda(std::shared_ptr<Object> key, std::shared_ptr<LambdaEvaluating> value);

void AddLambdaBB(std::shared_ptr<Object> key, std::shared_ptr<LambdaEvaluating> value);

void EraseLambda(std::shared_ptr<Object> key);

std::shared_ptr<Object> SugarDefinition(std::shared_ptr<Object> arguments);

std::shared_ptr<LambdaEvaluating> LambdaFind(std::shared_ptr<Object> key);

std::shared_ptr<LambdaEvaluating> LambdaFind(std::string key);