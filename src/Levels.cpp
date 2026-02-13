#include <iostream>
#include <ctime>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/Items.hpp"
#include "../include/Score.hpp"
#include <clocale>
#include <cstring>
#include <chrono>

#define DESTR_RATIO (10+(node->index*4))
#define ITEMS_RATIO 100
#define ENEMY_RATIO (1+(node->index*2))

using namespace std;

Itemlist items;
Player p(1, 1);

bool inBounds(int y, int x) {
    return y > 0 && y < 22 && x > 0 && x < 42;
}

double Levels::nowSec() {
    using namespace std::chrono;
    static const auto t0 = steady_clock::now();
    return duration<double>(steady_clock::now() - t0).count();
}

map* Levels::genlevels() {  //questa funzione genera i 5 livelli e ritorna un array che li contiene come matrici 23x43

    map* head = nullptr;
    map* prev = nullptr;

    for (int liv = 0; liv < 5; liv++) {  //creazione mura indistruttibili e inizializzazione lista
        map* node = new map;

        node->index = liv;
        node->previous = prev;
        node->next = nullptr;

        for (int y = 0; y < 23; y++)
            for (int x = 0; x < 43; x++) {
                if (x % 2 == 0 && y % 2 == 0)
                    node->level[y][x] = '#';
                else
                    node->level[y][x] = ' ';
                if (x == 0 || y == 0 || x == 42 || y == 22)
                    node->level[y][x] = '#';
            }

        if (prev)
            prev->next = node;
        else
            head = node;   // primo nodo
        prev = node;
    }

    srand(time(nullptr));     //generazione casuale di mura distruttibili (+) che dipende dal livello
    for (map* node = head; node; node = node->next) {
        int prob = DESTR_RATIO;
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (node->level[y][x]==' ' && (x>5 || y>5)) {
                    int p = rand()%100;
                    if (p<=prob)
                        node->level[y][x]='+';
                }
            }

    }

    for (map* node = head; node; node = node->next) {
        if (node->index !=0) node->level[1][0] = '<';
        if (node->index !=4) node->level[21][42] = '>';
    }

    //spawn nemici

    for (map* node = head; node; node = node->next) {

        bool bplaced = false;

        for (int y = 1; y < 22; y++) {
            for (int x = 1; x < 42; x++) {
                
                if (x <= 5 && y <= 5) // safe zone
                    continue;

                if (node->level[y][x] == ' ') {
                    int eprob;
                    if (node->index == 0) eprob = rand() % 110;
                    else eprob = rand() % 175;

                    if (eprob < ENEMY_RATIO){
                        int r = rand()%100;

                        if (r < 40) {              // nemico lento
                            node->level[y][x] = 'N';
                        }
                        else if (r < 65) {       // nemico veloce
                            node->level[y][x] = 'S';
                        } else if (r < 85) {
                            node->level[y][x] = 'T'; // nemico tank
                        } else if (r < 99 && !bplaced) {
                            node->level[y][x] = 'U'; // nemico bomberman
                            bplaced = true;
                        }
                    }
                }
            }
        }
    }
    return head;
}

//

WINDOW* Levels::enclose_screen(map* map, int time_left, int lvl) {  //questa funzione mostra su schermo la mappa (inizialmente livello 1)

    int x_offset = getmaxx(stdscr) / 2 - 21;
    if (x_offset < 0) x_offset = 0;

    WINDOW * screen = newwin(25, 45, 3, x_offset);

    printscreen(map, screen);

    mvwprintw(screen, 24, 29, "Tempo: %ds", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
    mvwprintw(screen, 24, 16, "Vite: %d",p.getLives());
    mvwprintw(screen, 0, 2, "Livello: %d", lvl + 1);

    wrefresh(screen);

    return screen;
}

//

map* Levels::change_level(map *head, WINDOW* screen, bool action, int lvl, int time_left, int lives) {

    map* node = head;
    while (node && node->index != lvl) {
        node = node->next;
    }
    if (!node) return head;

    printscreen(node, screen);

    mvwprintw(screen, 24, 29, "Tempo: %d s", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
    mvwprintw(screen, 24, 16, "Vite: %d", lives);
    mvwprintw(screen, 0, 2, "Livello: %d", lvl + 1);

    wrefresh(screen);

    return node;
}

void Levels::printscreen(map* level, WINDOW* screen) {
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

    for (int y=0; y<23; y++)
        for (int x=0; x<43; x++)
            switch (level->level[y][x]) {

                case '#':
                    mvwaddch(screen, y+1, x+1, ' ' | A_REVERSE);
                    break;

                case '+':
                    mvwaddch(screen, y+1, x+1, ACS_CKBOARD);
                    break;

            case 'N':
                    wattron(screen, COLOR_PAIR(4));
                    mvwaddch(screen, y+1, x+1, '~');
                    wattroff(screen, COLOR_PAIR(4));
                    break;

            case 'S':
                    wattron(screen, COLOR_PAIR(4));
                    mvwaddch(screen, y+1, x+1, '$');
                    wattroff(screen, COLOR_PAIR(4));
                    break;

            case 'T':
                    wattron(screen, COLOR_PAIR(4));
                    mvwaddch(screen, y+1, x+1, '%');
                    wattroff(screen, COLOR_PAIR(4));
                    break;

            case 'U':
                    wattron(screen, COLOR_PAIR(4));
                    mvwaddch(screen, y+1, x+1, '&');
                    wattroff(screen, COLOR_PAIR(4));
                    break;

            case '^':
                    wattron(screen, COLOR_PAIR(4));
                    mvwaddch(screen, y+1, x+1, '^');
                    wattroff(screen, COLOR_PAIR(4));
                    break;

                default:
                    mvwprintw(screen, y+1, x+1, "%c", level->level[y][x]);
                    break;
            }
    wattron(screen, COLOR_PAIR(5));
    items.hideitems(level, screen);
    wattroff(screen, COLOR_PAIR(5));
    wborder(screen, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
}


//----------------------------------------------------------------------------------------------------------------------


void Levels::run() {
    clear();
    noecho();
    box(stdscr, 0, 0);
    refresh();
    setlocale(LC_ALL, "");
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);

    double lastTime = nowSec();   // tempo precedente
    double time_left = 100.0;    // secondi di gioco
    time_t time_effect = 0;

    int EXPmult = 1; //valori gestione effetti
    bool updradius = false;
    char effect = 'a';
    bool affected[2][5]; //sintassi: wallbroken, killed, liv da 0 a 4

    for (int e = 0; e<2; e++)
        for (int l=0; l<5; l++)
            affected[e][l] = false;

    // tick
    int playerTick = 0;
    const int player_tick_delay = 1;

    // timer
    double lastEnemyN = nowSec();
    double lastEnemyS = nowSec();


    // invincibilità
    bool invincible = false;
    bool invincible_effect = false;
    int blinkCounter = 0;
    const int blink_delay = 11;
    int blinkTick = 0;

    char playerChar = '@';

    Map = genlevels();
    map* current_level = Map;

    p.setLives(3);
    p.resetScore();
    p.setPosition(1,1);

    WINDOW* screen = enclose_screen(Map, (int)time_left, 0);

    bool ingame = true;
    bool levelCleared[5] = { false, false, false, false, false };
    bool victory = false;
    int lvl = current_level->index;

    /* =====================================================
       BOMBA PLAYER
       ===================================================== */
    bool pBombPlaced = false;
    int  pBombX = -1, pBombY = -1;
    time_t pBombTime = 0;
    int bombRadius = 1;

    bool pExplosionVisible = false;
    time_t pExplosionTime = 0;
    int pExplosionX[32];
    int pExplosionY[32];
    int pExplosionCount = 0;

    /* =====================================================
       BOMBA NEMICO U
       ===================================================== */
    bool uBombPlaced = false;
    int  uBombX = -1, uBombY = -1;
    time_t uBombTime = 0;
    int uBombDelay = 2 + rand() % 3;
    int uBombRadius = 2;

    bool uExplosionVisible = false;
    time_t uExplosionTime = 0;
    int uExplosionX[32];
    int uExplosionY[32];
    int uExplosionCount = 0;

    /* ================= GAME LOOP ================= */
    while (ingame) {
        nodelay(screen, true);
        keypad(screen, true);
        napms(1); // evita saturazione CPU

        int ch = wgetch(screen);
        if (ch == ERR) ch = 0;

        int dx = 0, dy = 0;

        playerTick++;

        // difficoltà nemici
        double enemy_n_delay = 1.5 - current_level->index * 0.1;
        if (enemy_n_delay < 0.15) enemy_n_delay = 0.15;

        double enemy_s_delay = 1.10 - current_level->index * 0.05;
        if (enemy_s_delay < 0.05) enemy_s_delay = 0.05;
            
        // INPUT
        switch (ch) {
            case 27: // ESC
                ingame = false;
                continue;

            case 'w': case 'W': case KEY_UP:
                dy = -1; break;
            case 's': case 'S': case KEY_DOWN:
                dy = 1; break;
            case 'a': case 'A': case KEY_LEFT:
                dx = -1; break;
            case 'd': case 'D': case KEY_RIGHT:
                dx = 1; break;
            case 'e': case 'E':
                if (!pBombPlaced &&
                    current_level->level[p.getY()][p.getX()] == ' ') {

                    pBombPlaced = true;
                    pBombX = p.getX();
                    pBombY = p.getY();
                    pBombTime = time(nullptr);
                    current_level->level[pBombY][pBombX] = 'O';
                    }
                break;

            case 'p': case 'P': {
                time_t menu_span = time(nullptr);
                int x_offset = getmaxx(stdscr) / 2 - 21;
                if (x_offset < 0) x_offset = 0;

                WINDOW* controls = newwin(25, 45, 3, x_offset);
                nodelay(controls, true);
                box(controls, 0, 0);

                const char* logo[] = { // Scritta in ascii art
                  " ;8p;;  [[;[[  [[   ]]  ,d888  88P'''",
                  "';$[ ]; $$ '$. $$   $$ ;$s__   $$___ ",
                  " '89'' o88oo88 88,  88  ''98b, 88''' ",
                  " ,MY   UM' 'YP  PUMU9   YMMM9' YMMUYo"
                };

                int logoLines = 4;
                int logoWidth = 0;
                for (int i = 0; i < logoLines; i++) { // Calcolo larghezza della scritta (per centrarla)
                    int len = (int)strlen(logo[i]);
                    if (len > logoWidth) logoWidth = len;
                }

                int startX = (getmaxx(controls) - logoWidth) / 2; // Coordinate di dove si stampa il titolo
                int startY = 2;

                bool pulse = ((int)(nowSec()) % 2) == 0; // Pulsazione alternata (ogni secondo)
                if (pulse) wattron(controls, A_BOLD);
                for (int i = 0; i < logoLines; i++) { // Stampa del titolo
                    mvwprintw(controls, startY + i, startX, "%s", logo[i]);
                }
                if (pulse) wattroff(controls, A_BOLD);

                int startY2 = startY + logoLines + 3; // Stampa dei comandi
                mvwprintw(controls, startY2, 14, "COMANDI DI GIOCO");
                mvwprintw(controls, startY2 + 2, 2, "[WASD] / [Frecce]: movimento");
                mvwprintw(controls, startY2 + 3, 2, "[E]: piazza bomba");
                mvwprintw(controls, startY2 + 4, 2, "[ESC]: esci");
                mvwprintw(controls, startY2 + 6, 2, "Premere [P] o [ESC] per tornare al gioco");

                /* HUD */
                mvwprintw(controls, 24, 3,  "Punti: %d", p.getScore());
                mvwprintw(controls, 24, 16, "Vite: %d",  p.getLives());
                mvwprintw(controls, 24, 29, "Tempo: %d", (int)time_left);
                mvwprintw(controls, 0, 2, "Livello: %d", lvl + 1);

                char c;
                do {
                    c = wgetch(controls);
                } while (c != 27 && c != 'p' && c != 'P');

                delwin(controls);

                menu_span = time(nullptr) - menu_span;
                time_left += menu_span;

                screen = enclose_screen(current_level, (int)time_left, lvl);
                mvwprintw(screen, p.getY() + 1, p.getX() + 1, "%c", playerChar);
                break;
            }
        }

        // movimento player
        if ((dx || dy) && playerTick >= player_tick_delay) {

            int nx = p.getX() + dx;
            int ny = p.getY() + dy;

            char next = current_level->level[ny][nx];

            // player sul nemico
            if (next == 'N' || next == 'S' || next == 'T' || next == '^' || next == 'U') {
                if (!invincible && !invincible_effect) {
                    p.loseLife();
                    invincible = true;
                    blinkCounter = 0;
                    blinkTick = 0;
                }
                // NON entra nella cella
            }
            // movimento normale
            else if (next != '#' && next != '+') {
                p.move(dx, dy);
            }

            playerTick = 0;
        }

        // raccolta item
        if (current_level->level[p.getY()][p.getX()] >= 'a' &&
            current_level->level[p.getY()][p.getX()] <= 'z') {

            items.effect_list( current_level->level[p.getY()][p.getX()], current_level, screen, time(nullptr),
                &time_effect, &bombRadius, &invincible_effect, &p, &EXPmult, &updradius, affected);
            effect = current_level->level[p.getY()][p.getX()];

            current_level->level[p.getY()][p.getX()] = ' ';

            wattron(screen, COLOR_PAIR(2));
            mvwprintw(screen, p.getY() + 1, p.getX() + 1, "%c", playerChar);
            wattroff(screen, COLOR_PAIR(2));
            }

        // cambio livello
        if (current_level->level[p.getY()][p.getX()] == '<') {
            if (current_level->previous) {
                current_level = current_level->previous;
                lvl = current_level->index;
                p.setPosition(21, 41);

                map* nxt = current_level->next;
                if (nxt && levelCleared[nxt->index]) {
                    current_level->next = nxt->next;
                    if (nxt->next) nxt->next->previous = current_level;
                    time_left += 60;
                }
            }
        } else if (current_level->level[p.getY()][p.getX()] == '>') {
            if (current_level->next) {
                current_level = current_level->next;
                lvl = current_level->index;
                p.setPosition(1, 1);

                map* prev = current_level->previous;
                if (prev && levelCleared[prev->index]) {
                    current_level->previous = prev->previous;
                    if (prev->previous) prev->previous->next = current_level;
                    time_left += 60;
                }
            }
        }

        // =======================
        // ESPLOSIONE BOMBA PLAYER
        // =======================
        if (pBombPlaced && time(nullptr) - pBombTime >= 2) {

            pExplosionCount = 0;

            // centro
            pExplosionX[pExplosionCount] = pBombX;
            pExplosionY[pExplosionCount] = pBombY;
            pExplosionCount++;

            current_level->level[pBombY][pBombX] = ' ';

            int dx4[4] = {1, -1, 0, 0};
            int dy4[4] = {0, 0, 1, -1};

            for (int d = 0; d < 4; d++) {
                for (int r = 1; r <= bombRadius; r++) {

                    int ny = pBombY + dy4[d] * r;
                    int nx = pBombX + dx4[d] * r;

                    if (!inBounds(ny, nx)) break;

                    char &c = current_level->level[ny][nx];
                    if (c == '#') break;

                    if (pExplosionCount < 32) {
                        pExplosionX[pExplosionCount] = nx;
                        pExplosionY[pExplosionCount] = ny;
                        pExplosionCount++;
                    }

                    if (c == '+') {
                        p.addScore(100);
                        c = ' ';
                        break;
                    }

                    if ((c>='A'&&c<='Z')||c=='^') {
                        if (c == 'N') {
                            //c = ' ';
                            p.addScore(100 * EXPmult);
                        }
                        else if (c == 'S') {
                            //c = ' ';
                            p.addScore(150 * EXPmult);
                        }
                        else if (c == 'U') {
                            //c = ' ';
                            p.addScore(200 * EXPmult);
                        }
                        else if (c == 'T') {
                            c = '^';
                            p.addScore(100 * EXPmult);
                        }
                        else if (c == '^') {
                            c = items.spawnrate(ITEMS_RATIO);
                            p.addScore(50 * EXPmult);
                        }

                        if (c!='^')
                            c = items.spawnrate(ITEMS_RATIO);
                    }

                }
            }

            // danno al player
            bool damagedThisExplosion = false;

            for (int i = 0; i < pExplosionCount; i++) {
                if (pExplosionX[i] == p.getX() &&
                    pExplosionY[i] == p.getY()) {

                    if (!invincible && !invincible_effect && !damagedThisExplosion) {
                        p.loseLife();
                        invincible = true;
                        damagedThisExplosion = true;
                        blinkCounter = 0;
                        blinkTick = 0;
                    }
                    break;
                    }
            }

            pExplosionVisible = true;
            pExplosionTime = time(nullptr);
            pBombPlaced = false;
        }

        // =====================
        // MOVIMENTO NEMICI
        // =====================

        double now = nowSec();

        bool canMoveN = (now - lastEnemyN >= enemy_n_delay);
        bool canMoveS = (now - lastEnemyS >= enemy_s_delay);

        for (int y = 1; y < 22; y++) {
            for (int x = 1; x < 42; x++) {

                char enemy = current_level->level[y][x];

                if (enemy != 'N' && enemy != 'S' && enemy != 'T' &&
                    enemy != '^' && enemy != 'U')
                    continue;

                // velocità diverse
                if ((enemy == 'N' || enemy == 'T') && !canMoveN)
                    continue;

                if ((enemy == 'S' || enemy == '^' || enemy == 'U') && !canMoveS)
                    continue;

                int dir = rand() % 4;
                int nx = x, ny = y;

                if (dir == 0) ny--;
                if (dir == 1) ny++;
                if (dir == 2) nx--;
                if (dir == 3) nx++;

                if (!inBounds(ny, nx)) continue;

                // =====================
                // NEMICO U: piazza bomba
                // =====================
                if (enemy == 'U' && !uBombPlaced) {
                    int r = rand() % 25;
                    if (r < 2) {

                        // verifica che la cella di destinazione sia libera
                        if (current_level->level[ny][nx] == ' ') {

                            uBombPlaced = true;
                            uBombX = x;
                            uBombY = y;
                            uBombTime = time(nullptr);
                            uBombDelay = 2 + rand() % 3;

                            // lascia la bomba nella cella vecchia
                            wattron(screen, COLOR_PAIR(7));
                            current_level->level[y][x] = '=';
                            wattroff(screen, COLOR_PAIR(7));

                            // U si muove
                            current_level->level[ny][nx] = 'U';

                            continue;
                        }
                        // se non può muoversi → NON piazza la bomba
                    }
                }

                // =====================
                // COLLISIONE COL PLAYER
                // =====================
                if (nx == p.getX() && ny == p.getY()) {
                    if (!invincible && !invincible_effect) {
                        p.loseLife();
                        invincible = true;
                        blinkCounter = 0;
                        blinkTick = 0;
                    }
                    continue;
                }

                // =====================
                // MOVIMENTO NORMALE
                // =====================
                if (current_level->level[ny][nx] == ' ') {
                    current_level->level[ny][nx] = enemy;
                    current_level->level[y][x] = ' ';
                }
            }
        }

        if (canMoveN) lastEnemyN = now;
        if (canMoveS) lastEnemyS = now;


        // =======================
        // ESPLOSIONE BOMBA U
        // =======================
        if (uBombPlaced && time(nullptr) - uBombTime >= uBombDelay) {

            uExplosionCount = 0;

            // centro
            current_level->level[uBombY][uBombX] = ' ';
            uExplosionX[uExplosionCount] = uBombX;
            uExplosionY[uExplosionCount] = uBombY;
            uExplosionCount++;

            int dx4[4] = {1, -1, 0, 0};
            int dy4[4] = {0, 0, 1, -1};

            for (int d = 0; d < 4; d++) {
                for (int r = 1; r <= uBombRadius; r++) {

                    int ny = uBombY + dy4[d] * r;
                    int nx = uBombX + dx4[d] * r;

                    if (!inBounds(ny, nx)) break;

                    char &c = current_level->level[ny][nx];

                    // muro indistruttibile
                    if (c == '#')
                        break;

                    // muro distruttibile
                    if (c == '+') {
                        c = ' ';
                        uExplosionX[uExplosionCount] = nx;
                        uExplosionY[uExplosionCount] = ny;
                        uExplosionCount++;
                        break;
                    }

                    // cella libera
                    uExplosionX[uExplosionCount] = nx;
                    uExplosionY[uExplosionCount] = ny;
                    uExplosionCount++;
                }
            }

            // danno al player
            for (int i = 0; i < uExplosionCount; i++) {
                if (uExplosionX[i] == p.getX() &&
                    uExplosionY[i] == p.getY()) {

                    if (!invincible && !invincible_effect) {
                        p.loseLife();
                        invincible = true;
                        blinkCounter = 0;
                        blinkTick = 0;
                    }
                    break;
                    }
            }

            uExplosionVisible = true;
            uExplosionTime = time(nullptr);
            uBombPlaced = false;
        }



        int idx = current_level->index;

        // se il livello non è ancora stato clearato
        if ((!levelCleared[idx]) && countEnemies(current_level) == 0) {

            levelCleared[idx] = true;   // segna questo livello come completato

            // reset vite quando completi un livello
            p.setLives(3);

            // controlla se TUTTI i livelli sono stati completati
            bool allCleared = true;
            for (int i = 0; i < 5; i++) {
                if (!levelCleared[i]) {
                    allCleared = false;
                    break;
                }
            }

            // se tutti completati → vittoria
            if (allCleared) {
                victory = true;
                ingame = false;
            }
        }


        // =======================
        // PLAYER (BLINK)
        // =======================
        if ((invincible || invincible_effect) && (blinkCounter % 2 == 0))
            playerChar = ' ';
        else
            playerChar = '@';

        wattron(screen, COLOR_PAIR(2));
        mvwprintw(screen, p.getY() + 1, p.getX() + 1, "%c", playerChar);
        wattroff(screen, COLOR_PAIR(2));

        if (invincible || invincible_effect) {
            blinkTick++;
            if (blinkTick >= blink_delay) {
                blinkCounter++;
                blinkTick = 0;
            }

            if (blinkCounter >= 16) {   // durata invincibilità
                invincible = false;
                blinkCounter = 0;
                blinkTick = 0;
            }
        }

        //gestione portali d'accesso a livelli completati
        for (map* node = Map; node; node = node->next) {
            if (node->level[1][0] == '<' && !node->previous) {
                node->level[1][0] = '#';
            }
            if (node->level[21][42] == '>' && !node->next) {
                node->level[21][42] = '#';
            }
        }

            // =======================
            // TEMPO
            // =======================
            double currentTime = nowSec();
            double delta = currentTime - lastTime;
            lastTime = currentTime;

            time_left -= delta;
            if (time_left < 0.0)
                time_left = 0.0;

            if (time_effect && time(nullptr) >= time_effect) {
                items.reseteffects(screen, &bombRadius, &invincible_effect, &EXPmult, &updradius);
                time_effect = 0;
                effect = 'a';
            }
            if (p.getLives() <= 0 || time_left <= 0.0)
                ingame = false;

            // =======================
            // RENDER
            // =======================
            werase(screen);

            printscreen(current_level, screen);

            /* HUD */
            mvwprintw(screen, 24, 3,  "Punti: %d", p.getScore());
            mvwprintw(screen, 24, 16, "Vite: %d",  p.getLives());
            mvwprintw(screen, 24, 29, "Tempo: %d", (int)time_left);
            mvwprintw(screen, 0, 2, "Livello: %d", lvl + 1);
            items.printeffects(screen, effect, affected[0][current_level->index], affected[1][current_level->index], updradius );

            // =======================
            // ESPLOSIONE BOMBA PLAYER
            // =======================
            if (pExplosionVisible) {
                wattron(screen, COLOR_PAIR(3));

                for (int i = 0; i < pExplosionCount; i++) {
                    mvwprintw(screen,
                              pExplosionY[i] + 1,
                              pExplosionX[i] + 1,
                              "*");
                }

                wattroff(screen, COLOR_PAIR(3));

                if (time(nullptr) - pExplosionTime >= 1) {
                    pExplosionVisible = false;
                    pExplosionCount = 0;
                }
            }

            // =======================
            // ESPLOSIONE BOMBA U
            // =======================
            if (uExplosionVisible) {
                wattron(screen, COLOR_PAIR(3));

                for (int i = 0; i < uExplosionCount; i++) {
                    mvwprintw(screen,
                              uExplosionY[i] + 1,
                              uExplosionX[i] + 1,
                              "H");
                }

                wattroff(screen, COLOR_PAIR(3));

                if (time(nullptr) - uExplosionTime >= 1) {
                    uExplosionVisible = false;
                    uExplosionCount = 0;
                }
            }

            /* player dopo */
            wattron(screen, COLOR_PAIR(2));
            mvwprintw(screen, p.getY() + 1, p.getX() + 1, "%c", playerChar);
            wattroff(screen, COLOR_PAIR(2));

            if (levelCleared[current_level->index]) {
                mvwprintw(screen, 0, 14, "CLEAR");
            }
            wrefresh(screen);

        } // fine gameloop

        // bonus tempo se vittoria
        if (victory && time_left > 0) {
            p.addScore((int)time_left);
        }

        // reset input per inserimento nome
        echo();
        nocbreak();
        nodelay(screen, false);

        // schermata finale
        clear();

        if (victory) {
            mvprintw(10, 18, "YOU WIN!");
            mvprintw(12, 10, "Hai completato tutti i livelli!");
        } else {
            mvprintw(10, 18, "GAME OVER");
        }

        refresh();

        // prompt nome (COMUNE a entrambi i casi)
        char name[32];
        WINDOW* prompt = newwin(5, 40,
            getmaxy(stdscr)/2 - 2,
            getmaxx(stdscr)/2 - 20);

        while (true) {
            werase(prompt);
            box(prompt, 0, 0);
            mvwprintw(prompt, 1, 2, "Inserisci il tuo nome: ");
            wrefresh(prompt);
            wgetnstr(prompt, name, 31);
            bool hasSpace = false;
            for (int i = 0; name[i]; i++) {
                if (!isalnum(name[i])) {
                    hasSpace = true;    break;
                }
            }
            if (!hasSpace && strlen(name) > 0)    break;
            werase(prompt);
            box(prompt, 0, 0);
            mvwprintw(prompt, 1, 2, "Niente spazi nel nome!"); // Messaggio d'errore
            mvwprintw(prompt, 3, 2, "Premi [INVIO]...");
            wrefresh(prompt);
            wgetch(prompt);
        }

        Score::saveScore("Leaderboard.txt", name, p.getScore());

        delwin(prompt);

        // pulizia finale
        clear();
        refresh();

        // libera mappe
        map* tmp;
        while (Map) {
            tmp = Map;
            Map = Map->next;
            delete tmp;
        }

        delwin(screen);

}