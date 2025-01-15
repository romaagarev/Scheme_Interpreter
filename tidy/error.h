#pragma once

#include <stdexcept>

struct SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    explicit SyntaxError(const std::string& string) : runtime_error(string) {
    }
};

struct RuntimeError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    explicit RuntimeError(const std::string& string) : runtime_error(string) {
    }
};

struct NameError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    explicit NameError(const std::string& string) : runtime_error(string) {
    }
};
