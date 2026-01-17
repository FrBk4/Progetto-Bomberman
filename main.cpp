#include <curses.h>

int main() {
    initscr();
    printw("PDCurses funziona!");
    refresh();
    getch();
    endwin();
    return 0;
}
