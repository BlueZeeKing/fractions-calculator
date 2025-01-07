#pragma once

#include "Expression.h"
#include <variant>
#include <vector>

using namespace std;

enum Symbol {
    Plus,
    Minus,
    Star,
    Slash,
    ParenL,
    ParenR
};

Expression parse_expression(vector<variant<Symbol, Fraction>> input);
