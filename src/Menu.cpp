#include "Menu.h"
#include <curses.h>
#include <cstring>
#include <ctime>
using namespace std;

double Menu::nowSec() {
    return (double)clock() / CLOCKS_PER_SEC;
}

void Menu::initLogo() { // Inizializza il titolo
    const char* logo[] = { // Titolo in ascii art
        "#########   ########    ###   ###   #########  ########## #########    ###   ###       ###     ###     ###",
        "###    ### ###    ###  ##### #####  ###    ### ###        ###    ###  ##### #####    ### ###   ####    ###",
        "###    ### ###    ### ### ##### ### ###    ### ###        ###    ### ### ##### ###  ###   ###  #####   ###",
        "#########  ###    ### ###  ###  ### #########  #########  ########   ###  ###  ### ########### ### ### ###",
        "###    ### ###    ### ###       ### ###    ### ###        ###    ### ###       ### ###     ### ###   #####",
        "###    ### ###    ### ###       ### ###    ### ###        ###    ### ###       ### ###     ### ###    ####",
        "#########   ########  ###       ### #########  ########## ###    ### ###       ### ###     ### ###     ###"
    };

    logoWidth = 0;
    for (int i = 0; i < LOGO_LINES; i++) { // Calcolo larghezza del titolo (per centrarlo)
        int len = (int)strlen(logo[i]);
        if (len > logoWidth) logoWidth = len;

        strncpy(logoGrid[i], logo[i], LOGO_MAXW - 1); // Copia il titolo in una matrice (per poter salvare le distruzioni)
        logoGrid[i][LOGO_MAXW - 1] = '\0';
    }

    for (int y = 0; y < LOGO_LINES; y++)
        for (int x = 0; x < LOGO_MAXW; x++)
            flameTicks[y][x] = 0;

    logoReady = true; // Segnala che il titolo è stato disegnato e non serve più aggiornarlo fino a che non viene avviata nuova partita
}

void Menu::destroyCross(int xRel, int yRel, int R) { // Animazione per la distruzione del titolo per via delle bombe
    auto burn = [&](int x, int y) {
        if (y < 0 || y >= LOGO_LINES) return;
        if (x < 0 || x >= logoWidth) return;

        if (logoGrid[y][x] == '#') // Distrugge un blocco del titolo
            logoGrid[y][x] = ' ';

        flameTicks[y][x] = 12; // Accende la fiamma per 12 tick
    };

    burn(xRel, yRel);
    for (int d = 1; d <= R; d++) { // Calcolo della posizione dell'esplosione
        burn(xRel - d, yRel);
        burn(xRel + d, yRel);
        burn(xRel, yRel - d);
        burn(xRel, yRel + d);
    }
}

void Menu::startBombAnimation() { // Funzione che da il via a tutto il processo delle bombe
    if (!logoReady) initLogo(); // Inizializza il titolo in caso

    double t0 = nowSec(); // Tempo di inizio
    animActive = true;
    animEndAt = t0 + 2.6;

    srand(time(nullptr)); // Per la generazione della posizione delle bombe

    bombs[0] = { rand() % 100, rand() % 7, t0 + 0.8, false }; // Generazione bombe
    bombs[1] = { rand() % 100, rand() % 7, t0 + 1.3, false };
    bombs[2] = { rand() % 100, rand() % 7, t0 + 1.8, false };
}

void Menu::updateBombAnimation() {
    if (!animActive) return; // Se non è stata attivata l'animazione non c'è da fare niente

    double t = nowSec();

    for (int i = 0; i < 3; i++) { // Fa esplodere ogni bomba
        if (!bombs[i].exploded && t >= bombs[i].explodeAt) {
            bombs[i].exploded = true;
            destroyCross(bombs[i].xRel, bombs[i].yRel, 3);
        }
    }

    if (t >= animEndAt) { // L'animazione è finita
        animActive = false;
    }

    for (int y = 0; y < LOGO_LINES; y++) {
        for (int x = 0; x < logoWidth; x++) {
            if (flameTicks[y][x] > 0) flameTicks[y][x]--;
        }
    }
}

void Menu::draw(const string items[], // Gestisce la stampa delle varie cose
                int count,
                int selected,
                int key) {
    clear();

    int h, w; // Valore massimo di altezza e larghezza della finestra principale
    getmaxyx(stdscr, h, w);

    if (!logoReady) initLogo(); // Inizializza il titolo
    updateBombAnimation(); // Controlla che non sia da attivare l'animazione delle bombe

    int startX = (w - logoWidth) / 2; // Coordinate di dove si stampa il titolo
    int startY = 2;

    attron(A_BOLD);
    for (int i = 0; i < LOGO_LINES; i++) { // Stampa del titolo
        mvprintw(startY + i, startX, "%s", logoGrid[i]);
    }
    attroff(A_BOLD);

    attron(COLOR_PAIR(1)); // Colore delle fiamme
    for (int y = 0; y < LOGO_LINES; y++) { // Stampa delle fiamme
        for (int x = 0; x < logoWidth; x++) {
            if (flameTicks[y][x] > 0) {
                mvaddch(startY + y, startX + x, ' ');
            }
        }
    }
    attroff(COLOR_PAIR(1));

    if (animActive) { // Animazione delle bombe
        for (int i = 0; i < 3; i++) {
            int bx = startX + bombs[i].xRel;
            int by = startY + bombs[i].yRel;

            if (!bombs[i].exploded) {
                mvprintw(by, bx, "O"); // Bomba
            } else { // Fiamme dell'esplosione
                double t = nowSec();
                if (t < bombs[i].explodeAt + 0.2) {
                    mvprintw(by, bx, " ");
                }
            }
        }
    }

    if (key == int('p')) // Stampa dei comandi
        mvprintw(startY + LOGO_LINES + 1, 2, "SU/GIU o W/S per muovere, INVIO per scegliere, ESC per uscire");
    if (key == int('q')) { // Nasconde i comandi (premendo di nuovo "p")
        move (startY + LOGO_LINES + 1, 0);
        clrtoeol();
    }

    int startMenuY = h - count - 3; // Riga iniziale del menu

    for (int i = 0; i < count; i++) { // Stampa elementi del menu
        int x = 6;
        int y = startMenuY + i;

        if (i == selected) attron(A_REVERSE); // Evidenzia elemento selezionato
        mvprintw(y, x, "%s", items[i].c_str());
        if (i == selected) attroff(A_REVERSE);
    }
    refresh();
}

int Menu::run(const string items[], // Fa runnare tutto il menu e gestisce l'input utente
              int count) {
    int selected = 0;
    int key = 0;

    nodelay(stdscr, FALSE);

    while (true) {
        draw(items, count, selected, key); // Stampa menu e titolo iniziale

        int ch = getch(); //Input utente

        if (ch == int('p')) { // Mostra/nasconde comandi
            if (key == int('p'))
                key = int('q');
            else
                key = int('p');
        }

        if (ch == 27) { // Esc = esce dal menu
            return -1;
        }
        else if (ch == KEY_UP || ch == 'w') { // Movimento nel menu
            selected = (selected - 1 + count) % count;
        }
        else if (ch == KEY_DOWN || ch == 's') {
            selected = (selected + 1) % count;
        }
        else if (ch == KEY_ENTER || ch == '\n' || ch == 13) { // Enter per selezionare
            if (selected == 0) { // Se l'utente ha selezionato Nuova Partita
                startBombAnimation(); // Avvia animazione bombe sul titolo

                nodelay(stdscr, TRUE); // Non aspetta l'input utente per andare avanti con l'animazione

                while (animActive) {
                    draw(items, count, selected, key);
                    napms(16); // ~60 FPS
                }

                nodelay(stdscr, FALSE); // Torna ad aspettare per l'input dell'utente
            }
            return selected; // Infine ritorna a Game::run() cosa è stato selezionato
        }
    }
}
