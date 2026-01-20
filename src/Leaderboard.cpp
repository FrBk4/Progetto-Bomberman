#include "Leaderboard.h"
#include <curses.h>
#include <cstring>
#include <ctime>

double Leaderboard::nowSec() { // Calcola il tempo (serve per la pulsazione dell'ascii art)
    return (double)clock() / CLOCKS_PER_SEC;
}

void Leaderboard::destroyWindow() { // Distrugge la finestra (per quanto si torna nel menu principale)
    if (boardWin != nullptr) {
        delwin(boardWin);
        boardWin = nullptr;
    }
    initBoard = false;
}

void Leaderboard::initWindow(int startY) { // Inizializza la finestra dove viene disengata la classifica
    int hMax, wMax;
    getmaxyx(stdscr, hMax, wMax);

    if (initBoard && hMax == lastH && wMax == lastW) return; // Se la finestra è già inizializzata torna indietro

    destroyWindow();

    lastH = hMax;
    lastW = wMax;

    boardY = startY + 1; // Parte da sotto all'ascii art

    boardX = 5; // Coordinate di dove viene stampata la finestra
    boardW = wMax - 10;

    boardH = (hMax - 2) - boardY; // Altezza della finestra

    boardWin = newwin(boardH, boardW, boardY, boardX);
    box(boardWin, 0, 0); // Crea la cornice

    mvwprintw(boardWin, 2, 2, "Nome");
    mvwprintw(boardWin, 2, boardW - 10, "Punti");

    for (int x = 1; x < boardW - 1; x++)
        mvwaddch(boardWin, 3, x, ACS_HLINE);

    initBoard = true;
}

void Leaderboard::drawStatic() { // Disegna ciò che non cambia ogni frame

    if (!frameInit) { // Digna il bordo
        erase();
        box(stdscr, 0, 0);
        frameInit = true;
    }

    if (boardWin != nullptr) { // Ridisegno completo della classifica (solo quando ricreata)
        box(boardWin, 0, 0);

        mvwprintw(boardWin, 2, 2, "Nome");
        mvwprintw(boardWin, 2, boardW - 10, "Punti");

        for (int x = 1; x < boardW - 1; x++) // Disegna la riga di mezzo della classifica (quella sotto Nome e Punti)
            mvwaddch(boardWin, 3, x, ACS_HLINE);
    }
}

void Leaderboard::drawDynamic() { // Disegna ciò che cambia ogni frame (pulsazione + testo variabile)
    int hMax, wMax;
    getmaxyx(stdscr, hMax, wMax);

    const char* logo[] = { // Scritta in ascii art
        "[[   '88P'''  [[;[[  d88op, '88P''' [[[[;;, 888''b,  d88oop   [[;[[  [[[[;;, d88op,",
        "$$    $$___   $$ '$. $$  '$; $$___  $$[ ]$$ $$c__o  $$'  '$$  $$ '$. $$[ ]$$ $$  '$;",
        "88    88'''  o88oo88 88   8; 88'''  888;8o  88;''8o 88,  ,88 o88oo88 888;8o  88   8;",
        "YYMUo YMMUYo UM' 'YP YMoo8P 'YMMUYo YM 'MMo 'MMMMP   d8P,oP' UM' 'YP YM 'MMo YMoo8P"
    };

    logoWidth = 0;
    for (int i = 0; i < logoLines; i++) { // Calcolo larghezza della scritta (per centrarla)
        int len = (int)strlen(logo[i]);
        if (len > logoWidth) logoWidth = len;
    }

    int startX = (wMax - logoWidth) / 2; // Coordinate di dove si stampa il titolo
    int startY = 2;

    initWindow(startY + logoLines); // Inizializza la schermata della classifica

    for (int y = 0; y < logoLines; y++) { // Pulisce l'area del titolo (non tutta la linea)
        for (int x = 0; x < logoWidth; x++) {
            mvaddch(startY + y, startX + x, ' ');
        }
    }

    bool pulse = ((int)(nowSec()) % 2) == 0; // Pulsazione alternata (ogni secondo)
    if (pulse) attron(A_BOLD);
    for (int i = 0; i < logoLines; i++) { // Stampa del titolo
        mvprintw(startY + i, startX, "%s", logo[i]);
    }
    if (pulse) attroff(A_BOLD);

    if (boardWin != nullptr) {

        wattron(boardWin, A_BOLD);
        mvwprintw(boardWin, 1, (boardW - 10) / 2, "CLASSIFICA"); // Stampa della scritta di titolo della classifica
        wattroff(boardWin, A_BOLD);

        mvwprintw(boardWin, 5, 2, "1) AAA"); // Placeholder dei dati
        mvwprintw(boardWin, 5, boardW - 10, "1200");

        mvwprintw(boardWin, 6, 2, "2) BBB");
        mvwprintw(boardWin, 6, boardW - 10, "900");

        mvwprintw(boardWin, 7, 2, "3) CCC");
        mvwprintw(boardWin, 7, boardW - 10, "500");

        mvwprintw(boardWin, boardH - 2, 2, "[ESC] Torna indietro"); // Comando per uscire (si può uscire anche con INVIO)
    }
}

void Leaderboard::draw() { // Si occupa di chiamare i vari metodi per la grafica della schermata
    int hMax, wMax;
    getmaxyx(stdscr, hMax, wMax);

    drawStatic(); // Disegna cosa non cambia a ogni ciclo (le cornici)
    drawDynamic(); // Disegna cosa cambia a ogni ciclo (l'ascii art e i componenti del menu)

    refresh(); // Per lo schermo principale (con l'ascii art)
    if (boardWin != nullptr) wrefresh(boardWin); // Per lo schermo della classifica
}

void Leaderboard::run() {
    nodelay(stdscr, TRUE); // L'input non blocca così da far funzionare le animazioni

    frameInit = false;
    draw(); // Primo disegno del menu

    while (true) {
        int ch = getch(); // Input utente

        if (ch == KEY_ENTER || ch == '\n' || ch == 13 || ch == 27) { // Invio/Esc = esce
            destroyWindow(); // Quando si esce distrugge la finestra creata per la classifica
            break;
        }

        draw(); // Disegno della schermata leaderboard
        napms(16); // Limita la velocità del loop (per evitare flickering)
    }

    nodelay(stdscr, FALSE);
}
