#include "parse_expression.h"
#include "Expression.h"

Expression parse_expression_no_parens(vector<variant<Symbol, Fraction>>::iterator input, vector<variant<Symbol, Fraction>>::iterator end) {
    if (input == end) {
        throw "Ran out of inputs";
    }

    if (input->index() == 0 && get<Symbol>(*input) == Minus) {
        return Expression(Fraction(-1, 1)) * parse_expression_no_parens(input + 1, end);
    }

    if (input->index() == 0) {
        throw "Invalid operator";
    }

    if (input == end - 1) {
        return Expression(get<Fraction>(*input));
    }

    if ((input + 1)->index() == 1) {
        throw "Invalid fraction";
    }

    switch (get<Symbol>(*(input + 1))) {
    case Plus:
        return Expression(get<Fraction>(*input)) + parse_expression_no_parens(input + 2, end);
    case Minus:
        return Expression(get<Fraction>(*input)) - parse_expression_no_parens(input + 2, end);
    case Star:
        return Expression(get<Fraction>(*input)) * parse_expression_no_parens(input + 2, end);
    case Slash:
        return Expression(get<Fraction>(*input)) / parse_expression_no_parens(input + 2, end);
    default:
        throw "Invalid symbol";
    }
}

Expression parse_expression(vector<variant<Symbol, Fraction>> input) {
    vector<int> left_paren_stack;
    for (int i = 0; i < input.size(); i++) {
        if (input[i].index() == 0 && get<Symbol>(input[i]) == ParenL) {
            left_paren_stack.push_back(i);
        } else if (input[i].index() == 0 && get<Symbol>(input[i]) == ParenR) {
            if (left_paren_stack.size() == 0) {
                throw "Too many left parentheses";
            }
            int beginning_idx = left_paren_stack.back();
            left_paren_stack.pop_back();
            input[beginning_idx] = parse_expression_no_parens(input.begin() + beginning_idx + 1, input.begin() + i).evaluate();
            input.erase(input.begin() + beginning_idx + 1, input.begin() + i + 1);
            i = beginning_idx;
            if (i > 0 && input[i - 1].index() == 1) {
                input.insert(input.begin() + i, Star);
                i++;
            }
        }
    }

    if (left_paren_stack.size() != 0) {
        throw "Too many right parentheses";
    }
    
    return parse_expression_no_parens(input.begin(), input.end());
}
