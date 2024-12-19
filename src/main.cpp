#include "ncurses.h"
#include "Fraction.h"
#include "FractionBuilder.h"
#include <cassert>
#include <clocale>
#include <variant>
#include <vector>

using namespace std;

enum Operator {
    Add,
    Subtract,
    Multiply,
    Divide,
};

int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(-1);
    curs_set(0);
    keypad(stdscr, true);

    int current_col = 1;

    vector<variant<Fraction, Operator>> input;

    attr_on(A_BOLD, nullptr);
    mvaddstr(5, 2, "basic info");

    mvaddstr(5, 50, "basic keybinds");

    mvaddstr(5, 80, "fraction editing keybinds");
    attr_off(A_BOLD, nullptr);
    attr_on(A_DIM, nullptr);
    mvaddstr(6, 2, "- this follows order of operations");
    mvaddstr(7, 2, "- you cannot delete or go back (yet)");
    mvaddstr(8, 5, "except inside a fraction");
    mvaddstr(9, 2, "- number keys in normal mode also");
    mvaddstr(10, 5, "start creating a fraction");

    mvaddstr(6, 50, "f | create fraction");
    mvaddstr(7, 50, "+ | add");
    mvaddstr(8, 50, "- | subtract");
    mvaddstr(9, 50, "* | multiply");
    mvaddstr(10, 50, "/ | divide");
    mvaddstr(11, 50, "= | calculate");

    mvaddstr(6, 80, "arrow keys | move around");
    mvaddstr(7, 80, "num keys   | type numbers");
    mvaddstr(8, 80, "enter      | next line or complete fraction");
    mvaddstr(9, 80, "backspace  | delete");
    mvaddstr(10, 80, "/          | go to next line");
    attr_off(A_DIM, nullptr);

    Fraction fraction;

    while(true) {
        mvchgat(2, current_col, 1, A_STANDOUT, 0, nullptr);
        
        int ch = getch();
        switch (ch) {
        case '+':
            input.push_back(Operator::Add);
            mvaddch(2, current_col, '+');
            current_col += 2;
            break;
        case '-':
            input.push_back(Operator::Subtract);
            mvaddch(2, current_col, '-');
            current_col += 2;
            break;
        case '*':
            input.push_back(Operator::Multiply);
            mvaddch(2, current_col, '*');
            current_col += 2;
            break;
        case '/':
            input.push_back(Operator::Divide);
            mvaddch(2, current_col, '/');
            current_col += 2;
            break;
        case 'f': 
            fraction = build_fraction(1, current_col);
            current_col += fraction.output(1, current_col) + 1;
            input.push_back(fraction);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            fraction = build_fraction(1, current_col, stdscr, string(1, ch));
            current_col += fraction.output(1, current_col) + 1;
            input.push_back(fraction);
            break; 
        }

        if (ch == '=') {
            mvaddch(2, current_col, '=');
            current_col += 2;
            break;
        }

        refresh();
    }

    for (auto i = input.begin() + 1; i < input.end() - 1; i++) {
        if (i->index() == 0 || (get<Operator>(*i) != Operator::Multiply && get<Operator>(*i) != Operator::Divide)) {
            continue;
        }

        Fraction first = get<Fraction>(*(i - 1));
        Operator op = get<Operator>(*i);
        Fraction second = get<Fraction>(*(i + 1));

        input.erase(i);
        input.erase(i);
        if (op == Operator::Multiply) {
            *(i - 1) = first * second;
        } else {
            *(i - 1) = first / second;
        }
        i--;
    }

    for (auto i = input.begin() + 1; i < input.end() - 1; i++) {
        if (i->index() == 0 || (get<Operator>(*i) != Operator::Add && get<Operator>(*i) != Operator::Subtract)) {
            continue;
        }

        Fraction first = get<Fraction>(*(i - 1));
        Operator op = get<Operator>(*i);
        Fraction second = get<Fraction>(*(i + 1));

        input.erase(i);
        input.erase(i);
        if (op == Operator::Add) {
            *(i - 1) = first + second;
        } else {
            *(i - 1) = first - second;
        }
        i--;
    }

    for (auto i = input.begin(); i < input.end(); i++) {
        char symbol;

        switch (i->index()) {
        case 0:
            current_col += get<Fraction>(*i).output(1, current_col);
            break;
        case 1:
            switch (get<Operator>(*i)) {
            case Add:
                symbol = '+';
                break;
            case Subtract:
                symbol = '-';
                break;
            case Multiply:
                symbol = '*';
                break;
            case Divide:
                symbol = '/';
                break;
            }
            mvaddch(2, current_col, symbol);
            current_col += 2;
            break;
        }
    }

    getch();

    endwin();
}
