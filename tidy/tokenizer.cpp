#include <tokenizer.h>

SymbolToken::SymbolToken(const std::string& string) : name(string){};
bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
};
bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

ConstantToken::ConstantToken(int number) : value(number){};
bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}
BooleanToken::BooleanToken(const std::string& boolean_string) {
    if (boolean_string == "#t") {
        value = true;
    } else {
        value = false;
    }
}

BooleanToken::BooleanToken(bool a) : value(a){};

bool BooleanToken::operator==(const BooleanToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) : stream_(in) {
    current_char_ = in->get();
    Next();
};

bool Tokenizer::IsEnd() {
    return is_end_;
}

bool CorrectSymbolStart(char a) {
    return (a - 'a' >= 0 && a - 'a' <= 25) || (a - 'A' >= 0 && a - 'A' <= 25) || a == '>' ||
           a == '=' || a == '<' || a == '*' || a == '/' || a == '#';
}

bool CorrectCharInSymbol(char a) {
    return CorrectSymbolStart(a) || isdigit(a) || a == '!' || a == '?' || a == '-';
}

bool IsSpace(char a) {
    return a == ' ' || a == '\n';
}

void Tokenizer::Next() {
    if (IsEnd()) {
        return;
    }
    std::string current_string;
    char a = current_char_;
    while (IsSpace(a)) {
        a = stream_->get();
    }
    if (a == EOF) {
        is_end_ = true;
        return;
    }
    if (a == '(') {
        current_token_ = BracketToken::OPEN;
        current_char_ = stream_->get();
        return;
    }
    if (a == ')') {
        current_token_ = BracketToken::CLOSE;
        current_char_ = stream_->get();
        return;
    }
    if (a == '\'') {
        current_token_ = QuoteToken();
        current_char_ = stream_->get();
        return;
    }
    if (a == '.') {
        current_token_ = DotToken();
        current_char_ = stream_->get();
        return;
    }
    if (a == '+') {
        if (ConstantToken* x = std::get_if<ConstantToken>(&current_token_)) {
            current_token_ = SymbolToken("+");
            current_char_ = stream_->get();
            return;
        }
        current_string.push_back(a);
        a = stream_->get();
        if (!isdigit(a)) {
            current_token_ = SymbolToken("+");
            current_char_ = a;
            return;
        } else {
            current_string.pop_back();
            current_string.push_back(a);
            a = stream_->get();
            while (isdigit(a)) {
                current_string.push_back(a);
                a = stream_->get();
            }
            current_char_ = a;
            current_token_ = ConstantToken(std::stoi(current_string));
            return;
        }
    }
    if (a == '-') {
        current_string.push_back(a);
        a = stream_->get();
        if (!isdigit(a)) {
            current_token_ = SymbolToken("-");
            current_char_ = a;
            return;
        } else {
            current_string.push_back(a);
            a = stream_->get();
            while (isdigit(a)) {
                current_string.push_back(a);
                a = stream_->get();
            }
            current_char_ = a;
            current_token_ = ConstantToken(std::stoi(current_string));
            return;
        }
    }
    if (isdigit(a)) {
        while (isdigit(a)) {
            current_string.push_back(a);
            a = stream_->get();
        }
        current_token_ = ConstantToken(std::stoi(current_string));
        current_char_ = a;
        return;
    }
    if (CorrectSymbolStart(a)) {
        current_string.push_back(a);
        a = stream_->get();
        while (CorrectCharInSymbol(a)) {
            current_string.push_back(a);
            a = stream_->get();
        }
        if (current_string == "#t" || current_string == "#f") {
            current_token_ = BooleanToken(current_string);
        } else {
            current_token_ = SymbolToken(current_string);
        }
        current_char_ = a;
        return;
    }
}

Token Tokenizer::GetToken() {
    return current_token_;
}
