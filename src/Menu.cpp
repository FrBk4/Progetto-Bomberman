#include "Menu.h"
#include <curses.h>
#include <cstring>
using namespace std;

void Menu::draw(const string items[], // Gestisce la stampa delle varie cose
                int count,
                int selected,
                int key) {
    clear();

    int h, w;
    getmaxyx(stdscr, h, w);

    const char* logo[] = { // Salva il titolo in una variabile
        "______                 _                                    ",
        "| ___ \\               | |                                   ",
        "| |_/ / ___  _ __ ___ | |__   ___ _ __ _ __ ___   __ _ _ __ ",
        "| ___ \\/ _ \\| '_ ` _ \\| '_ \\ / _ \\ '__| '_ ` _ \\ / _` | '_ \\",
        "| |_/ / (_) | | | | | | |_) |  __/ |  | | | | | | (_| | | | |",
        "\\____/ \\___/|_| |_| |_|_.__/ \\___|_|  |_| |_| |_|\\__,_|_| |_|"
    };
    int logoLines = 6; // Altezza del titolo

    int logoWidth = 0;
    for (int i = 0; i < logoLines; i++) { // Calcolo lunghezza del titolo (per centrarlo)
        int len = (int)strlen(logo[i]);
        if (len > logoWidth) logoWidth = len;
    }

    int startX = (w - logoWidth) / 2;
    int startY = 1;

    for (int i = 0; i < logoLines; i++) { // Stampa del titolo
        mvprintw(startY + i, startX, "%s", logo[i]);
    }
    if (key == int('p')) // Stampa dei comandi
        mvprintw(startY + logoLines + 1, 2, "SU/GIU o W/S per muovere, INVIO per scegliere, ESC per uscire");
    if (key == int('q')) { // Nasconde i comandi (premendo di nuovo "p")
        move (startY + logoLines + 1, 0);
        clrtoeol();
    }


    int startMenuY = startY + logoLines + 4; // Riga iniziale del menu

    for (int i = 0; i < count; i++) { // Stampa elementi del menu
        int x = (w - items[i].length()) / 2;
        int y = startMenuY + i;

        if (i == selected) attron(A_REVERSE); // Evidenzia elemento selezionato
        mvprintw(y, x, "%s", items[i].c_str());
        if (i == selected) attroff(A_REVERSE);
    }
    refresh();
}


int Menu::run(const string items[], // Gestisce input utente dentro al menu e chiama draw()
              int count) {
              int selected = 0;
              int key = 0;

    nodelay(stdscr, FALSE);

    while (true) {
        draw(items, count, selected, key); // Chiama la stampa

        int ch = getch();
        if (ch == int('p')) { // Gestisce il far comparire/nascondere la lista dei comandi
            if (key == int('p'))
                key = int('q');
            else
                key = int('p');
        }

        if (ch == 27) { // Ascii di Esc
            return -1;
        } else if (ch == KEY_UP || ch == 'w') { // Gestione movimento dentro al menu
            selected = (selected - 1 + count) % count;
        } else if (ch == KEY_DOWN || ch == 's') {
            selected = (selected + 1) % count;
        } else if (ch == KEY_ENTER || ch == '\n' || ch == 13) { // Copre i vari Enter che possono arrivare
            return selected;
        }
    }
}