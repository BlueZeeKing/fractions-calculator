#include <variant>
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

    Expression(const Fraction);
    Expression(const Expression&);
    Expression(const Expression&, const Operation, const Expression&);
    ~Expression();

    Expression multiply_by(const Expression&) const;
    Expression divide_by(const Expression&) const;
    Expression subtract_by(const Expression&) const;
    Expression add_by(const Expression&) const;

    Expression* right_most_literal();

    Fraction evaluate() const;
    string to_string(int indentation = 0) const;
};
