#include "Game.h"
#include <curses.h>
#include <string>
using namespace std;

void Game::initCurses() { // Funzioni base per far andare nCurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, COLOR_RED);

}

void Game::shutdownCurses() { // Funzione per chiudere nCurses
    endwin();
}

void Game::run() { // Gestisce il gameloop
    initCurses(); // Fa partire nCurses

    string items[3]; // Elenco delle voci del menu
    items[0] = "Nuova partita";
    items[1] = "Classifica";
    items[2] = "Quit";

    int itemCount = 3;

    while (true) { // Gestisce il risultato dell'input dentro al menu
        int choice = menu_.run(items, itemCount); // Chiama run() finché l'utente non seleziona qualcosa dal menu

        if (choice == -1 || choice == 2) { // -1 quando Esc, 2 quando l'utente sceglie "quit" dal menu
            break;
        }

        clear();
        if (choice == 0) { // Da qua bisogna poi far partire la mappa e il gioco
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
