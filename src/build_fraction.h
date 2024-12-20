#pragma once

#include <ncurses.h>
#include "Fraction.h"
#include <string>
using namespace std;

enum Error {
    ShouldQuit,
    ShouldClear,
};

Fraction build_fraction(int row, int col, WINDOW* = stdscr, string initial_num = "");
