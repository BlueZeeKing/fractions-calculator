#include <algorithm>
#include <ncurses.h>
#include <string>
#include "Fraction.h"
#include "FractionBuilder.h"
using namespace std;

Fraction build_fraction(int row, int col, WINDOW* win, string initial_num) {
    bool is_at_top = true;
    string num = initial_num;
    string den = "";
    int cursor_pos = num.length();

    int currentLength;

    while (true) {
        for (int current_col = col; current_col < col + max(num.length(), den.length()) + 1; current_col++) {
            mvwaddch(win, row + 1, current_col, '-');
        }

        mvwaddstr(win, row, col, num.c_str());
        mvwaddstr(win, row + 2, col, den.c_str());

        mvwchgat(stdscr, is_at_top ? row : row + 2, col + cursor_pos, 1, A_STANDOUT, 0, nullptr);

        refresh();

        int ch = getch();    

        mvwchgat(stdscr, is_at_top ? row : row + 2, col + cursor_pos, 1, A_NORMAL, 0, nullptr);

        for (int current_col = col; current_col < col + max(num.length(), den.length()) + 1; current_col++) {
            for (int current_row = row; current_row <= row + 2; current_row++) {
                mvwaddch(win, current_row, current_col, ' ');
            }
        }

        switch (ch) {
        case KEY_UP:
            is_at_top = true;
            cursor_pos = min((int) num.length(), cursor_pos);
            break;
        case '/':
        case KEY_DOWN:
            is_at_top = false;
            cursor_pos = min((int) den.length(), cursor_pos);
            break;
        case '\n':
        case KEY_ENTER:
            if (is_at_top) {
                is_at_top = false;
                cursor_pos = min((int) den.length(), cursor_pos);
            } else {
                return Fraction(stoi(num), stoi(den));
            }
            break;
        case KEY_LEFT:
            cursor_pos--;
            if (cursor_pos < 0 && !is_at_top) {
                cursor_pos = num.length();
                is_at_top = true;
            } else if (cursor_pos < 0) {
                cursor_pos = 0;
            }
            break;
        case KEY_RIGHT:
            cursor_pos++;
            currentLength = is_at_top ? num.length() : den.length();
            if (cursor_pos > currentLength && is_at_top) {
                cursor_pos = 0;
                is_at_top = false;
            } else if (cursor_pos > currentLength) {
                cursor_pos = currentLength;
            }
            break;
        case KEY_BACKSPACE:
            if (cursor_pos == 0) {
                break;
            }
            if (is_at_top) {
                num.erase(num.begin() + cursor_pos - 1);
            } else {
                den.erase(den.begin() + cursor_pos - 1);
            }
            cursor_pos--;
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
            if (is_at_top) {
                num.insert(num.begin() + cursor_pos, ch);
            } else {
                den.insert(den.begin() + cursor_pos, ch);
            }
            cursor_pos++;
            break; 
        case '-':
            if (is_at_top && cursor_pos == 0) {
                num.insert(num.begin(), '-');
            } else if (cursor_pos == 0) {
                den.insert(den.begin(), '-');
            }
            cursor_pos++;
            break;
        }
    }
}
