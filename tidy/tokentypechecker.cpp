#include "tokentypechecker.h"

DotToken* AsDotToken(Token& token) {
    return std::get_if<DotToken>(&token);
}

QuoteToken* AsQuoteToken(Token& token) {
    return std::get_if<QuoteToken>(&token);
}

BracketToken* AsOPENBranchToken(Token& token) {
    BracketToken* x = std::get_if<BracketToken>(&token);
    if (x) {
        if (*x == BracketToken::OPEN) {
            return x;
        } else {
            return nullptr;
        }
    }
    return x;
}

BracketToken* AsCLOSEBranchToken(Token& token) {
    BracketToken* x = std::get_if<BracketToken>(&token);
    if (x) {
        if (*x == BracketToken::CLOSE) {
            return x;
        } else {
            return nullptr;
        }
    }
    return x;
}

ConstantToken* AsConstantToken(Token& token) {
    return std::get_if<ConstantToken>(&token);
}

BooleanToken* AsBooleanToken(Token& token) {
    return std::get_if<BooleanToken>(&token);
}

SymbolToken* AsSymbolToken(Token& token) {
    return std::get_if<SymbolToken>(&token);
}