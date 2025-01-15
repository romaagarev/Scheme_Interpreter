#include <parser.h>

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("");
    }
    Token token = tokenizer->GetToken();
    if (AsOPENBranchToken(token)) {
        tokenizer->Next();
        return ReadList(tokenizer, true);
    }
    if (AsBooleanToken(token)) {
        tokenizer->Next();
        return (new Boolean(*AsBooleanToken(token)))->GetRef();
    }
    if (AsConstantToken(token)) {
        tokenizer->Next();
        return (new Number(*AsConstantToken(token)))->GetRef();
    }
    if (AsSymbolToken(token)) {
        tokenizer->Next();
        auto to_make = (new Symbol(*AsSymbolToken(token)))->GetRef();
        //        if(Contains(to_make) && Is<Symbol>(Get(to_make))){
        //            to_make = Get(to_make);
        //        }
        return to_make;
    }
    if (AsQuoteToken(token)) {
        tokenizer->Next();
        token = SymbolToken("quote");
        auto quote = (new Symbol(*AsSymbolToken(token)))->GetRef();
        auto arg = Read(tokenizer);
        auto after_cell = (new Cell(arg, nullptr))->GetRef();
        return (new Cell(quote, after_cell))->GetRef();
    }
    throw SyntaxError("");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer, bool is_brackets) {
    Token token = tokenizer->GetToken();
    if (AsCLOSEBranchToken(token)) {
        if (is_brackets) {
            tokenizer->Next();
        }
        return nullptr;
    }
    auto first = Read(tokenizer);
    if (tokenizer->IsEnd()) {
        if (is_brackets) {
            throw SyntaxError("");
        } else {
            return (new Cell(first, nullptr))->GetRef();
        }
    }
    token = tokenizer->GetToken();
    if (AsDotToken(token)) {
        tokenizer->Next();
        token = tokenizer->GetToken();
        if (tokenizer->IsEnd() || AsCLOSEBranchToken(token)) {
            throw SyntaxError("");
        }
        auto second = Read(tokenizer);
        token = tokenizer->GetToken();
        if (tokenizer->IsEnd() || !AsCLOSEBranchToken(token)) {
            throw SyntaxError("");
        }
        if (is_brackets) {
            tokenizer->Next();
        }
        return (new Cell(first, second, true))->GetRef();
    } else {
        if (AsCLOSEBranchToken(token)) {
            if (is_brackets) {
                tokenizer->Next();
            }

            return (new Cell(first, nullptr))->GetRef();
        }
        auto second = ReadList(tokenizer, false);
        if (!is_brackets) {
            return (new Cell(first, second))->GetRef();
        }
        token = tokenizer->GetToken();
        if (tokenizer->IsEnd() || !AsCLOSEBranchToken(token)) {
            throw SyntaxError("");
        }
        tokenizer->Next();
        return (new Cell(first, second))->GetRef();
    }
}
