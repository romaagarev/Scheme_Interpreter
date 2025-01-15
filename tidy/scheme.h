#pragma once

#include <string>
#include "parser.h"
#include <sstream>
#include "running.h"
#include "unordered_map"
#include "variables.h"
#include "object.h"

auto ReadF(const std::string& str);

std::string Print(std::shared_ptr<Object> result);

class Interpreter {
public:
    std::string Run(const std::string&);
    ~Interpreter();
};
