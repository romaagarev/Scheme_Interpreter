#pragma once

#include <memory>
#include "heap.h"
#include "tokenizer.h"
#include <unordered_set>

class Object : public std::enable_shared_from_this<Object> {
private:
    std::unordered_set<std::shared_ptr<Object>> dependant_;
    bool marked_ = false;

protected:
    void AddDependant(std::shared_ptr<Object> son);

    void DeleteDependant(std::shared_ptr<Object> son);

public:
    void Mark();
    bool IsMarked();
    void ResetMark();
    Object();
    virtual ~Object();
    virtual std::shared_ptr<Object> GetRef();
    // virtual std::shared_ptr<Object> Eval(std::shared_ptr<Object> arguments) = 0;
    void ResetAll();
};

class Number : public Object {
public:
    Number(ConstantToken& token);
    int GetValue() const;
    Number(int val);

private:
    int value_;
};

class Symbol : public Object {
public:
    Symbol(SymbolToken& token);
    Symbol(std::string name);
    const std::string& GetName() const;
    void SetName(std::string name);

private:
    std::string name_;
};

class Boolean : public Object {
public:
    Boolean(BooleanToken& token);
    Boolean(bool val);
    bool GetValue() const;

private:
    bool value_;
};

class Cell : public Object {
public:
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second);
    Cell(std::shared_ptr<Object> other);
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second, bool br);
    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;
    void SetFirst(std::shared_ptr<Object> val);
    void SetSecond(std::shared_ptr<Object> val);
    bool IsDotted();

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
    bool dotted_ = false;
};

void StartMarks();

void SpecialDelete(std::shared_ptr<Object>& val);

void NewDelete();

void ResetMarks();

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    if (std::dynamic_pointer_cast<T>(obj)) {
        return true;
    }
    return false;
}
