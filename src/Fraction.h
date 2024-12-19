#pragma once

#include <ncurses.h>

struct Fraction {
    int numerator;
    int denominator;

    Fraction();
    Fraction(int, int);

    void reduce();

    void print() const;
    Fraction operator+(const Fraction&) const;
    Fraction operator-(const Fraction&) const;
    Fraction operator*(const Fraction&) const;
    Fraction operator/(const Fraction&) const;
    int output(int row, int col, WINDOW* window = stdscr);
};
