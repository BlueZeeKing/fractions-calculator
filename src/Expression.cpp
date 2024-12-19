#include "Expression.h"
#include <iostream>
#include <sstream>

Expression::Expression(const Fraction literal) {
    this->literal = literal;
    is_literal = true;
    left = nullptr;
    right = nullptr;
}

Expression::Expression(const Expression& expression) {
    if (expression.is_literal) {
        literal = expression.literal;
        is_literal = expression.is_literal;
    } else {
        operation = expression.operation;
        left = new Expression(*expression.left);
        right = new Expression(*expression.right);
    }
}

Expression::Expression(const Expression& left, const Operation operation, const Expression& right) {
    this->left = new Expression(left);
    this->right = new Expression(right);
    this->operation = operation;
    is_literal = false;
}

Expression* Expression::right_most_literal() {
    if (is_literal) {
        return this;
    }

    return right->right_most_literal();
}

Expression Expression::multiply_by(const Expression& other) const {
    Expression result(*this); 
    Expression* right_most = result.right_most_literal();
    right_most->left = new Expression(*right_most);
    right_most->right = new Expression(other);
    right_most->operation = Multiply;
    right_most->is_literal = false;
    return result;
}

Expression Expression::divide_by(const Expression& other) const {
    Expression result(*this); 
    Expression* right_most = result.right_most_literal();
    Expression* right_most_copy = new Expression(*right_most);
    right_most->left = right_most_copy;
    right_most->right = new Expression(other);
    right_most->operation = Divide;
    right_most->is_literal = false;
    return result;
}

Expression Expression::add_by(const Expression& other) const {
    return Expression(*this, Add, other);
}

Expression Expression::subtract_by(const Expression& other) const {
    return Expression(*this, Subtract, other);
}

Expression::~Expression() {
    if (!is_literal) {
        delete left;
        delete right;
    }
}

Fraction Expression::evaluate() const {
    if (is_literal) {
        return literal; 
    }
    
    switch (operation) {
    case Add:
        return left->evaluate() + right->evaluate();
    case Subtract:
        return left->evaluate() - right->evaluate();
    case Multiply:
        return left->evaluate() * right->evaluate();
    case Divide:
    default:
        return left->evaluate() / right->evaluate();
    }
}

string Expression::to_string(int indentation) const {
    stringstream output;
    for (int i = 0; i < indentation; i++) {
        output << " ";
    }
    if (is_literal) {
        output << literal.numerator << "/" << literal.denominator;
    } else {
        switch (operation) {
        case Add:
            output << "add:\n";
            break;
        case Subtract:
            output << "subtract:\n";
            break;
        case Multiply:
            output << "multiply:\n";
            break;
        case Divide:
        default:
            output << "divide:\n";
            break;
        }
        output << left->to_string(indentation + 3) << '\n' << right->to_string(indentation + 3);
    }

    return output.str();
}
