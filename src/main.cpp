#include "ncurses.h"
#include "Fraction.h"
#include "Expression.h"
#include "build_fraction.h"
#include "parse_expression.h"
#include <cassert>
#include <clocale>
#include <variant>
#include <vector>

using namespace std;

void draw_help() {
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

    mvaddstr(6, 50, "f  | create fraction");
    mvaddstr(7, 50, "+  | add");
    mvaddstr(8, 50, "-  | subtract");
    mvaddstr(9, 50, "*  | multiply");
    mvaddstr(10, 50, "/  | divide");
    mvaddstr(10, 50, "() | parentheses");
    mvaddstr(11, 50, "=  | calculate");

    mvaddstr(6, 80, "arrow keys | move around");
    mvaddstr(7, 80, "num keys   | type numbers");
    mvaddstr(8, 80, "enter      | next line or complete fraction");
    mvaddstr(9, 80, "backspace  | delete");
    mvaddstr(10, 80, "/          | go to next line");
    mvaddstr(11, 80, "c          | clear");
    mvaddstr(12, 80, "q          | quit");
    attr_off(A_DIM, nullptr);
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    start_color();
    timeout(-1);
    curs_set(0);
    keypad(stdscr, true);

    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);

    draw_help();

    int current_col = 1, ch;
    bool should_quit = false;

    vector<variant<Symbol, Fraction>> input;

    Fraction fraction;

    while (true) {
    start:

        current_col = 1;
        should_quit = false;
        input.clear();

        while(true) {
            mvchgat(2, current_col, 1, A_STANDOUT, 0, nullptr);
            
            ch = getch();
            switch (ch) {
            case '+':
                input.push_back(Plus);
                mvaddch(2, current_col, '+');
                current_col += 2;
                break;
            case '-':
                input.push_back(Minus);
                mvaddch(2, current_col, '-');
                current_col += 2;
                break;
            case '*':
                input.push_back(Star);
                mvaddch(2, current_col, '*');
                current_col += 2;
                break;
            case '/':
                input.push_back(Slash);
                mvaddch(2, current_col, '/');
                current_col += 2;
                break;
            case '(':
                input.push_back(ParenL);
                mvaddch(2, current_col, '|');
                mvaddch(1, current_col, '/');
                mvaddch(3, current_col, '\\');
                current_col += 1;
                break;
            case ')':
                input.push_back(ParenR);
                mvaddch(2, current_col, ' ');
                mvaddch(2, current_col-1, '|');
                mvaddch(3, current_col-1, '/');
                mvaddch(1, current_col-1, '\\');
                current_col += 1;
                break;
            case 'f': 
                try {
                    fraction = build_fraction(1, current_col);
                } catch (Error e) {
                    switch (e) {
                        case ShouldQuit:
                            goto end;
                        case ShouldClear:
                            clear();
                            draw_help();
                            goto start;
                    }
                }
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
                try {
                    fraction = build_fraction(1, current_col, stdscr, string(1, (char) ch));
                } catch (Error e) {
                    switch (e) {
                        case ShouldQuit:
                            goto end;
                        case ShouldClear:
                            clear();
                            draw_help();
                            goto start;
                    }
                }
                current_col += fraction.output(1, current_col) + 1;
                input.push_back(fraction);
                break; 
            case 'c':
                clear();
                draw_help();
                goto start;
            case 'q':
                goto end;
            }

            if (ch == '=') {
                mvaddch(2, current_col, '=');
                current_col += 2;
                break;
            }

            refresh();
        }

        try {
            parse_expression(input).evaluate().output(1, current_col);
        } catch (const char* error) {
            color_set(1, nullptr);
            mvaddstr(2, current_col, error);
            color_set(0, nullptr);
        }

        while (true) {
            switch (getch()) {
            case 'c':
                clear();
                draw_help();
                goto start;
            case 'q':
                goto end;
            }
        }
    }

    end:
    endwin();
}
