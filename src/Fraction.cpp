#include "Fraction.h"
#include <algorithm>
#include <iostream>
#include <ncurses.h>
using namespace std;

Fraction::Fraction() {
    numerator = 0;
    denominator = 1;
}

Fraction::Fraction(int numerator, int denominator) {
    this->numerator = numerator;
    this->denominator = denominator;
}

void Fraction::reduce() {
    for (int i = denominator; i > 1; i--) {
        if (numerator % i == 0 && denominator % i == 0) {
            numerator /= i;
            denominator /= i;

            break;
        }
    }
}

void Fraction::print() const {
    cout << numerator << "/" << denominator << endl;
}

Fraction Fraction::operator+(const Fraction& other) const {
    Fraction sum(other.denominator * this->numerator + this->denominator * other.numerator, other.denominator * this->denominator);
    sum.reduce();
    return sum;
}

Fraction Fraction::operator-(const Fraction& other) const {
    return *this + other * Fraction(-1, 1);
}

Fraction Fraction::operator*(const Fraction& other) const {
    Fraction product(other.numerator * this->numerator, other.denominator * this->denominator);
    product.reduce();
    return product;
}

Fraction Fraction::operator/(const Fraction& other) const {
    return *this * Fraction(other.denominator, other.numerator);
}

int Fraction::output(int row, int col, WINDOW* win) {
    string num = std::to_string(this->numerator);
    string den = std::to_string(this->denominator);
    int length = max(num.length(), den.length());
    if (length == 1) { // makes it look better
        length = 3;
    }
    mvwaddstr(win, row, col + (length - num.length()) / 2, num.c_str());
    mvwaddstr(win, row+2, col + (length - den.length()) / 2, den.c_str());
    for (int current_col = col; current_col < col + length; current_col++) {
        mvwaddch(win, row + 1, current_col, '-');
    }
    return length;
}
