#pragma once

#include "Expression.h"
#include <variant>
#include <vector>

using namespace std;

Expression parse_expression(vector<variant<Operation, Fraction>>&);
