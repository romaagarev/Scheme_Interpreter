#pragma once

#include <memory>

#include "object.h"
#include <tokenizer.h>
#include "lambda.h"
#include "variables.h"
#include "tokentypechecker.h"

std::shared_ptr<Object> Read(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer, bool is_brackets);