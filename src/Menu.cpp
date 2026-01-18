#include "Menu.h"
#include <curses.h>
using namespace std;

void Menu::draw(const string& title,
                const string items[],
                int count,
                int selected) {
    clear();

    int h, w;
    getmaxyx(stdscr, h, w);

    // Titolo
    mvprintw(2, (w - title.length()) / 2, "%s", title.c_str());
    mvprintw(4, 2, "SU/GIU o W/S per muovere, INVIO per scegliere, ESC per uscire");

    int startY = 7;
    for (int i = 0; i < count; i++) {
        int x = (w - items[i].length()) / 2;
        int y = startY + i;

        if (i == selected) attron(A_REVERSE);
        mvprintw(y, x, "%s", items[i].c_str());
        if (i == selected) attroff(A_REVERSE);
    }

    refresh();
}

int Menu::run(const string& title,
              const string items[],
              int count) {
    int selected = 0;

    nodelay(stdscr, FALSE);

    while (true) {
        draw(title, items, count, selected);

        int ch = getch();

        if (ch == 27) { // Ascii di Esc
            return -1;
        } else if (ch == KEY_UP || ch == 'w') {
            selected = (selected - 1 + count) % count;
        } else if (ch == KEY_DOWN || ch == 's') {
            selected = (selected + 1) % count;
        } else if (ch == KEY_ENTER || ch == '\n' || ch == 13) { // Copre i vari Enter che possono arrivare
            return selected;
        }
    }
}
