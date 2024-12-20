#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "Fraction.h"
#include "Expression.h"
#include "parse_expression.h"

TEST_CASE("Testing constructors") {
    Fraction fraction;
    CHECK_EQ(fraction.numerator, 0);
    CHECK_EQ(fraction.denominator, 1);

    Fraction fraction2(5, 10);
    CHECK_EQ(fraction2.numerator, 5);
    CHECK_EQ(fraction2.denominator, 10);
}

TEST_CASE("Testing plus") {
    Fraction fraction(5, 10);
    Fraction fraction2(15, 10);
    Fraction sum = fraction + fraction2;
    CHECK_EQ(sum.numerator, 2);
    CHECK_EQ(sum.denominator, 1);

    fraction = Fraction(1, 1);
    fraction2 = Fraction(1, 6);
    sum = fraction + fraction2;
    CHECK_EQ(sum.numerator, 7);
    CHECK_EQ(sum.denominator, 6);
}

TEST_CASE("Testing minus") {
    Fraction fraction(5, 10);
    Fraction fraction2(15, 10);
    Fraction difference = fraction - fraction2;
    CHECK_EQ(difference.numerator, -1);
    CHECK_EQ(difference.denominator, 1);

    fraction = Fraction(1, 1);
    fraction2 = Fraction(1, 6);
    difference = fraction - fraction2;
    CHECK_EQ(difference.numerator, 5);
    CHECK_EQ(difference.denominator, 6);
}

TEST_CASE("Testing times") {
    Fraction fraction(5, 10);
    Fraction fraction2(15, 10);
    Fraction product = fraction * fraction2;
    CHECK_EQ(product.numerator, 3);
    CHECK_EQ(product.denominator, 4);

    fraction = Fraction(2, 1);
    fraction2 = Fraction(1, 6);
    product = fraction * fraction2;
    CHECK_EQ(product.numerator, 1);
    CHECK_EQ(product.denominator, 3);
}

TEST_CASE("Testing divide by") {
    Fraction fraction(5, 10);
    Fraction fraction2(15, 10);
    Fraction quotient = fraction / fraction2;
    CHECK_EQ(quotient.numerator, 1);
    CHECK_EQ(quotient.denominator, 3);

    fraction = Fraction(2, 1);
    fraction2 = Fraction(1, 6);
    quotient = fraction / fraction2;
    CHECK_EQ(quotient.numerator, 12);
    CHECK_EQ(quotient.denominator, 1);
}

TEST_CASE("Testing expression creation") {
    Fraction value = Expression(Fraction(1,1)).divide_by(Fraction(3, 1)).add_by(Fraction(2, 1)).divide_by(Fraction(3, 1)).evaluate();

    CHECK_EQ(value.numerator, 1);
    CHECK_EQ(value.denominator, 1);
}

TEST_CASE("Testing expression parsing") {
    vector<variant<Operation, Fraction>> input;
    input.push_back(Fraction(1, 1));
    input.push_back(Multiply);
    input.push_back(Fraction(1, 3));
    input.push_back(Add);
    input.push_back(Fraction(2, 1));
    input.push_back(Divide);
    input.push_back(Fraction(3, 1));
    Fraction value = parse_expression(input).evaluate();

    CHECK_EQ(value.numerator, 1);
    CHECK_EQ(value.denominator, 1);
}
