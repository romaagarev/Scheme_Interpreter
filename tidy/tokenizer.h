#pragma once

#include <variant>
#include <optional>
#include <istream>
#include <ctype.h>
#include "error.h"

struct SymbolToken {
    std::string name;
    SymbolToken(const std::string& string);
    SymbolToken() = default;
    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    QuoteToken() = default;
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    DotToken() = default;
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;
    ConstantToken(int number);
    ConstantToken() = default;
    bool operator==(const ConstantToken& other) const;
};

struct BooleanToken {
    bool value;
    BooleanToken(const std::string& boolean_string);
    BooleanToken() = default;
    BooleanToken(bool a);
    bool operator==(const BooleanToken& other) const;
};

using Token =
    std::variant<BooleanToken, ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    std::istream* stream_;
    Token current_token_;
    char current_char_;
    bool is_end_ = false;
};

bool CorrectSymbolStart(char a);

bool CorrectCharInSymbol(char a);

bool IsSpace(char a);
