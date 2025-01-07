#pragma once

#include <string>
#include "Fraction.h"
using namespace std;

enum Operation {
    Add,
    Subtract,
    Multiply,
    Divide
};

struct Expression {
    Expression *left;
    Expression *right;
    Operation operation;
    Fraction literal;
    bool is_literal;

    Expression();
    Expression(const Fraction);
    Expression(const Expression&);
    Expression(const Expression&, const Operation, const Expression&);
    ~Expression();
    Expression& operator=(const Expression&);

    Expression operator*(const Expression&) const;
    Expression operator/(const Expression&) const;
    Expression operator-(const Expression&) const;
    Expression operator+(const Expression&) const;

    void operator*=(const Expression&);
    void operator/=(const Expression&);
    void operator-=(const Expression&);
    void operator+=(const Expression&);

    Expression* right_most_literal();

    Fraction evaluate() const;
    string to_string(int indentation = 0) const;
};
