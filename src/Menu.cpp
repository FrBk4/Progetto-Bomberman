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
        ".:::::::.      ...     .        :  .:::::::.   ,:::::: :::::::..   .        :     :::.    '::.    :::.",
        " ';;;'';;.  .;;;;;;;.  ;;,.    ;;;  ';;;'';;.  ;;;'''' ;;;;``;;;;  ;;,.    ;;;    ;;`;;    ;;;;,  `;;;",
        "  [[[,,[[;. [['    '[[ [[[[, ,[[[[,  [[[,,[[;. [[cccc   [[[,/[[['  [[[[, ,[[[[,  ,[[ '[[,   [[[[[. '[[",
        "  $$''''Y$$ $$,     $$ $$$$$$$$'$$$  $$''''Y$$ $$''''   $$$$$$c    $$$$$$$$'$$$  $$$cc$$$c  ;$$''Y$c$$.",
        "  88o,,od8P 888,_ _,88 888 Y88' 888o 88o,,od8P 888oo,_  888b '88b  888 Y88' 888o o88  '888o o88'  'Y88;",
        " ''YUMMMP'   'YMMMMMP' MMM  M'  'MMM 'YUMMMP'  'YYYUMM YMMM   'WYP MMM  M'  'MMM MM'   YMMY MM'     YMb"};

    /*const char* logo[] = { // Titolo base (se si vuole cambiare titolo ricordarsi di mettere LOGO_LINES = 7 in menu.h)
        "#########   ########    ###   ###   #########  ########## #########    ###   ###       ###     ###     ###",
        "###    ### ###    ###  ##### #####  ###    ### ###        ###    ###  ##### #####    ### ###   ####    ###",
        "###    ### ###    ### ### ##### ### ###    ### ###        ###    ### ### ##### ###  ###   ###  #####   ###",
        "#########  ###    ### ###  ###  ### #########  #########  ########   ###  ###  ### ########### ### ### ###",
        "###    ### ###    ### ###       ### ###    ### ###        ###    ### ###       ### ###     ### ###   #####",
        "###    ### ###    ### ###       ### ###    ### ###        ###    ### ###       ### ###     ### ###    ####",
        "#########   ########  ###       ### #########  ########## ###    ### ###       ### ###     ### ###     ###"};*/

    logoWidth = 0;
    for (int i = 0; i < LogoLines; i++) { // Calcolo larghezza del titolo (per centrarlo)
        int len = (int)strlen(logo[i]);
        if (len > logoWidth) logoWidth = len;

        strncpy(logoGrid[i], logo[i], LOGO_MAXW - 1); // Copia il titolo in una matrice (per poter salvare le distruzioni)
        logoGrid[i][LOGO_MAXW - 1] = '\0';
    }

    for (int y = 0; y < LogoLines; y++)
        for (int x = 0; x < LOGO_MAXW; x++)
            flameTicks[y][x] = 0;

    logoReady = true; // Segnala che il titolo è stato disegnato e non serve più aggiornarlo fino a che non viene avviata nuova partita
}

void Menu::destroyCross(int xRel, int yRel, const int R) { // Animazione per la distruzione del titolo per via delle bombe
    auto burn = [&](int x, int y) {
        if (y < 0 || y >= LogoLines) return;
        if (x < 0 || x >= logoWidth) return;

        logoGrid[y][x] = ' ';

        flameTicks[y][x] = 12; // Accende la fiamma per 12 tick
    };

    burn(xRel, yRel);
    for (int d = 1; d <= R; d++) { // Calcolo della posizione dell'esplosione
        burn(xRel - d, yRel);
        burn(xRel + d, yRel);
        if (d != 3) {
            burn(xRel, yRel - d);
            burn(xRel, yRel + d);
        }
    }
}

void Menu::startBombAnimation() { // Funzione che da il via a tutto il processo delle bombe
    if (!logoReady) initLogo(); // Inizializza il titolo in caso

    double t0 = nowSec(); // Tempo di inizio
    animActive = true;
    animEndAt = t0 + 2.6;

    srand(time(nullptr)); // Per la generazione della posizione delle bombe

    bombs[0] = { rand() % logoWidth, rand() % LogoLines, t0 + 0.8, false }; // Generazione bombe
    bombs[1] = { rand() % logoWidth, rand() % LogoLines, t0 + 1.3, false };
    bombs[2] = { rand() % logoWidth, rand() % LogoLines, t0 + 1.8, false };
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

    for (int y = 0; y < LogoLines; y++) {
        for (int x = 0; x < logoWidth; x++) {
            if (flameTicks[y][x] > 0) flameTicks[y][x]--;
        }
    }
}

void Menu::draw(const string items[], // Gestisce la stampa delle varie cose
                int countItems,
                int selected,
                int key,
                const string subtitles[],
                int countSubt) {
    clear();
    if (startTime < 0.0) // Inizia a contare il tempo solo una volta
        startTime = nowSec();

    box(stdscr, 0, 0); // Outline del menu

    int hMax, wMax; // Valore massimo di altezza e larghezza della finestra principale
    getmaxyx(stdscr, hMax, wMax);

    if (!logoReady) initLogo(); // Inizializza il titolo
    updateBombAnimation(); // Controlla che non sia da attivare l'animazione delle bombe

    int startX = (wMax - logoWidth) / 2; // Coordinate di dove si stampa il titolo
    int startY = 4;

    bool pulse = ((int)(nowSec()) % 2) == 0; // Pulsazione alternata (ogni secondo)
    if (pulse) attron(A_BOLD);
    for (int i = 0; i < LogoLines; i++) { // Stampa del titolo
        mvprintw(startY + i, startX, "%s", logoGrid[i]);
    }
    if (pulse) attroff(A_BOLD);

    attron(COLOR_PAIR(1)); // Colore delle fiamme
    for (int y = 0; y < LogoLines; y++) { // Stampa delle fiamme
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
        mvprintw(1, 2, "SU/GIU o W/S per muovere, INVIO per scegliere, ESC per uscire");
    if (key == int('q')) { // Nasconde i comandi (premendo di nuovo "p")
        move (2, 0);
        clrtoeol();
        box(stdscr, 0, 0);
    }

    int startMenuY = hMax - countItems - 4; // Riga iniziale del menu

    for (int i = 0; i < countItems; i++) { // Stampa elementi del menu
        int x = 9; // Colonna iniziale del menu
        int y = startMenuY + i;

        mvprintw(startMenuY + selected, x - 2, "O"); // Cursore di fianco
        if (i == selected) attron(A_REVERSE); // Evidenzia elemento selezionato
        mvprintw(y, x, "%s", items[i].c_str()); // Stampa elementi del menu
        if (i == selected) attroff(A_REVERSE);
    }

    int i = (int)((nowSec() - startTime) / 7.0); // Ciclo dei sottotitoli
    if (i >= countSubt) i = 0;
    mvprintw(hMax - 2, wMax - size(subtitles[i]) - 1 , "%s", subtitles[i].c_str()); // Stampa sottotitoli
    refresh();
}

int Menu::run(const string items[], // Fa runnare tutto il menu e gestisce l'input utente
              int countItems,
              const string subtitles[],
              int countSubt) {
    int selected = 0;
    int key = 0;

    while (true) {
        draw(items, countItems, selected, key, subtitles, countSubt); // Stampa menu e titolo iniziale

        nodelay(stdscr, TRUE); // Non aspetta l'input utente (per far andare avanti a prescindere l'animazione del titolo)
        int ch = getch(); // Input utente

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
            selected = (selected - 1 + countItems) % countItems;
        }
        else if (ch == KEY_DOWN || ch == 's') {
            selected = (selected + 1) % countItems;
        }
        else if (ch == KEY_ENTER || ch == '\n' || ch == 13) { // Enter per selezionare
            if (selected == 0) { // Se l'utente ha selezionato Nuova Partita
                startBombAnimation(); // Avvia animazione bombe sul titolo

                nodelay(stdscr, TRUE); // Non aspetta l'input utente per andare avanti con l'animazione

                while (animActive) {
                    draw(items, countItems, selected, key, subtitles, countSubt);
                    napms(16); // ~60 FPS
                }
            }
            nodelay(stdscr, FALSE); // Col cambio di schermata torna ad aspettare l'input utente
            return selected; // Infine ritorna a Game::run() cosa è stato selezionato
        }
    }
}
