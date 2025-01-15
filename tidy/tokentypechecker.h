#pragma once
#include "tokenizer.h"

DotToken* AsDotToken(Token& token);

QuoteToken* AsQuoteToken(Token& token);

BracketToken* AsOPENBranchToken(Token& token);

BracketToken* AsCLOSEBranchToken(Token& token);

ConstantToken* AsConstantToken(Token& token);

BooleanToken* AsBooleanToken(Token& token);

SymbolToken* AsSymbolToken(Token& token);