#include "Game.h"
#include <curses.h>
#include <string>
using namespace std;

void Game::initCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void Game::shutdownCurses() {
    endwin();
}

void Game::run() {
    initCurses();

    string items[3];
    items[0] = "Nuova partita";
    items[1] = "Classifica";
    items[2] = "Uscita";

    int itemCount = 3;

    while (true) {
        int choice = menu_.run(items, itemCount);

        if (choice == -1 || choice == 2) {
            break;
        }

        clear();
        if (choice == 0) {
            mvprintw(1, 2, "Hai scelto: Nuova partita");
        } else if (choice == 1) {
            mvprintw(1, 2, "Hai scelto: Classifica");
        }
        mvprintw(3, 2, "Premi un tasto per tornare al menu...");
        refresh();
        getch();
    }

    shutdownCurses();
}
