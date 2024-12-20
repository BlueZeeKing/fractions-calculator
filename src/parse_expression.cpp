#include "parse_expression.h"

Expression parse_expression(vector<variant<Operation, Fraction>>& input) {
    if (input.size() <= 2) {
        throw "Not enough inputs";
    }

    if (input[0].index() == 0) {
        throw "Input cannot start with an operator";
    }

    Expression expression = get<Fraction>(input[0]);

    for (auto i = input.begin() + 2; i < input.end(); i += 2) {
        if (i->index() == 0) {
            throw "Operator in an invalid position";
        }
        if ((i - 1)->index() == 1) {
            throw "Fraction in an invalid position";
        }

        switch (get<Operation>(*(i - 1))) {
        case Add:
            expression = expression.add_by(get<Fraction>(*i));
            break;
        case Subtract:
            expression = expression.subtract_by(get<Fraction>(*i));
            break;
        case Multiply:
            expression = expression.multiply_by(get<Fraction>(*i));
            break;
        case Divide:
            expression = expression.divide_by(get<Fraction>(*i));
            break;
        }
    }

    return expression;
}
