#include <iostream>
#include <ctime>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/Items.hpp"
#include "../include/Score.hpp"

#define DESTR_RATIO (10+(node->index*4))
#define ITEMS_RATIO (node->index+2)

using namespace std;

Itemlist items;
Player p(1, 1);

bool inBounds(int y, int x) {
    return y >= 0 && y < 22 && x >= 0 && x < 42;
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

    srand(time(nullptr));     //generazione casuale di mura distruttibili (=+) che dipende dal livello
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

    srand(time(nullptr));     //generazione casuale di items
    for (map* node = head; node; node = node->next) {
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (node->level[y][x]==' ' && (x>5 || y>5))
                    node->level[y][x] = items.spawnrate(ITEMS_RATIO);
            }

    }

    for (map* node = head; node; node = node->next) {
        if (node->index !=0) node->level[1][1] = '<';
        if (node->index !=4) node->level[21][41] = '>';
    }

    //spawn nemici

    for (map* node = head; node; node = node->next) {

        int placed = 0;

        for (int y = 1; y < 22 && placed < 5; y++) {
            for (int x = 1; x < 42 && placed < 5; x++) {
                
                if (x <= 5 && y <= 5) // safe zone
                    continue;

                if (node->level[y][x] == ' ') {
                    int r = rand() % 100;

                    if (r < 5) {              // nemico lento
                        node->level[y][x] = 'N';
                        placed++;
                    }
                    else if (r == 99) {       // nemico veloce
                        node->level[y][x] = 'S';
                        placed++;
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

    for (int y=0; y<23; y++)
        for(int x=0; x<43; x++) {
            mvwprintw(screen, y+1, x+1, "%c", map->level[y][x]);
        }

    items.hideitems(map, screen);

    wborder(screen, 186, 186, 205, 205, 201, 187, 200, 188);
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

    for (int y=0; y<23; y++)
        for(int x=0; x<43; x++) {
            mvwprintw(screen, y+1, x+1, "%c", node->level[y][x]);
        }

    items.hideitems(node, screen);

    wborder(screen, 186, 186, 205, 205, 201, 187, 200, 188);
    mvwprintw(screen, 24, 29, "Tempo: %d s", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
    mvwprintw(screen, 24, 16, "Vite: %d", lives);
    mvwprintw(screen, 0, 2, "Livello: %d", lvl + 1);


    wrefresh(screen);

    return node;
}


//----------------------------------------------------------------------------------------------------------------------


void Levels::run() {

    clear();
    box(stdscr, 0, 0);
    refresh();
    initscr();
    start_color();
    use_default_colors();
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    time_t start = time(nullptr); //gestione tempo
    time_t time_left = 1000;
    time_t start_effect = 0;

    // gestione ticks
    int playerTick = 0;
    int enemyNTick = 0;
    int enemySTick = 0;
    const int player_tick_delay = 1; // più basso = più veloce
    
    // invincibilità
    bool invincible = false;
    int blinkCounter = 0;
    const int maxBlink = 16;
    const int blink_delay = 100;
    int blinkTick = 0;

    // player assoluto
    char playerChar = '@';

    Map = genlevels();  //funzioni di generazione della mappa
    map* current_level = Map;

    p.setLives(3);
    p.resetScore();
    p.setPosition(1,1);

    WINDOW* screen = enclose_screen(Map, (int)time_left, 0);
    keypad(screen, true); //impostazioni dell'input
    nodelay(screen, TRUE);
    
    // input loop
    bool ingame = true;
    bool levelCleared[5] = { false, false, false, false, false };
    bool victory = false;
    int lvl = current_level->index;

    // bomba
    bool bombPlaced = false;
    int bombX = -1, bombY = -1;
    time_t bombTime = 0;
    int bombRadius = 2;

    // esplosione
    bool explosionVisible = false;
    time_t explosionTime = 0;
    int explosionX[32];
    int explosionY[32];
    int explosionCount = 0;

    while (ingame) {
        napms(1); //solo SOLO per non saturare CPU

        int ch = wgetch(screen);
        if (ch == ERR) ch = 0;

        int dx = 0, dy = 0;
        bool damagedThisFrame = false;

        playerTick++;
        enemyNTick++;
        enemySTick++;

        // livello difficoltà
        int enemy_n_delay = 1000 - current_level->index * 150; // lento
        if (enemy_n_delay < 200) enemy_n_delay = 200;

        int enemy_s_delay = 300 - current_level->index * 40; // veloce
        if (enemy_s_delay < 80) enemy_s_delay = 80;


        // input
        switch (ch) {
            case 27:
                ingame = false;
                continue;
            case 'w': case 'W': case KEY_UP:    dy = -1; break;
            case 's': case 'S': case KEY_DOWN:  dy =  1; break;
            case 'a': case 'A': case KEY_LEFT:  dx = -1; break;
            case 'd': case 'D': case KEY_RIGHT: dx =  1; break;
            case 'e': case 'E':
                if (!bombPlaced &&
                    current_level->level[p.getY()][p.getX()] == ' ') {

                    bombPlaced = true;
                    bombX = p.getX();
                    bombY = p.getY();
                    bombTime = time(nullptr);
                    current_level->level[bombY][bombX] = 'o';
                }
                break;
        } //fine input loop

        // movimento player
        if ((dx || dy) && playerTick >= player_tick_delay) {

            int nx = p.getX() + dx;
            int ny = p.getY() + dy;

            if (inBounds(ny, nx)) {
            char next = current_level->level[ny][nx];

            // player sul nemico
            if ((next == 'N' || next == 'S')) {
                if (!invincible) {
                    p.loseLife();
                    invincible = true;
                    blinkCounter = 0;
                    blinkTick = 0;
                }
                // il player non entra nella cella
            }
            // movimento normale
            else if (next != '#' && next != '+') {
                p.move(dx, dy);
            }
        }


            playerTick = 0;
        }

        //fetch item
        if (current_level->level[p.getY()][p.getX()]>=65 && current_level->level[p.getY()][p.getX()]<=90) {
            int lives = p.getLives();
            int radius = bombRadius;
            items.effect_list(current_level->level[p.getY()][p.getX()], &lives, current_level, screen, start, &start_effect, &bombRadius);
            p.setLives(lives);
            bombRadius = radius;
            current_level->level[p.getY()][p.getX()] = ' ';
            wattron(screen, COLOR_PAIR(2));
            mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", playerChar);
            wattroff(screen, COLOR_PAIR(2));
        }


        //cambio di livello
        if (current_level->level[p.getY()][p.getX()] == '<' && current_level->index > 0) {
            current_level = current_level->previous;
            lvl = current_level->index;
            p.setPosition(21, 40);
        }
        else if (current_level->level[p.getY()][p.getX()] == '>' && current_level->index < 4) {
            current_level = current_level->next;
            lvl = current_level->index;
            p.setPosition(1, 2);
        }



        // esplosione bomba
        if (bombPlaced && time(nullptr) - bombTime >= 2) {

            explosionCount = 0;
            explosionX[explosionCount] = bombX;
            explosionY[explosionCount] = bombY;
            explosionCount++;

            current_level->level[bombY][bombX] = ' ';

            int dx4[4] = {1, -1, 0, 0};
            int dy4[4] = {0, 0, 1, -1};

            for (int d = 0; d < 4; d++) {
                for (int r = 1; r <= bombRadius; r++) {

                    int ny = bombY + dy4[d] * r;
                    int nx = bombX + dx4[d] * r;

                    if (!inBounds(ny, nx))
                        break;

                    char &c = current_level->level[ny][nx];
                    if (c == '#') break;

                    if (c == '+') {
                        c = ' ';
                        p.addScore(20);
                        break;
                    }

                    if (explosionCount < 32) {
                        explosionX[explosionCount] = nx;
                        explosionY[explosionCount] = ny;
                        explosionCount++;
                    }

                    if (c == 'N') {
                        c = ' ';
                        p.addScore(100);
                    }

                    if (c == 'S') {
                        c = ' ';
                        p.addScore(150);
                    }
                }
            }

            // danno esplosione player
            if (!invincible) {
                for (int i = 0; i < explosionCount; i++) {
                    if (explosionX[i] == p.getX() &&
                        explosionY[i] == p.getY()) {
                        
                        p.loseLife();
                        invincible = true;
                        blinkCounter = 0;
                        blinkTick = 0;
                        break;
                    }
                }
            }

            explosionVisible = true;
            explosionTime = time(nullptr);
            bombPlaced = false;
        }

        // movimento nemici
        for (int y = 1; y < 22; y++) {
            for (int x = 1; x < 42; x++) {

                char enemy = current_level->level[y][x];
                if (enemy != 'N' && enemy != 'S')
                continue;
                
                if (enemy == 'N' && enemyNTick < enemy_n_delay) continue;
                if (enemy == 'S' && enemySTick < enemy_s_delay) continue;
                
                int dir = rand() % 4;
                int nx = x, ny = y;
                
                if (dir == 0) ny--;
                if (dir == 1) ny++;
                if (dir == 2) nx--;
                if (dir == 3) nx++;
                
                if (!inBounds(ny, nx))
                continue;
                
                // se il nemico tenta di entrare nel player
                if (nx == p.getX() && ny == p.getY()) {
                    if (!invincible) {
                        p.loseLife();
                        invincible = true;
                        blinkCounter = 0;
                        blinkTick = 0;
                    }
                    continue; // il nemico NON si muove
                }

                // movimento normale
                if (current_level->level[ny][nx] == ' ') {
                    current_level->level[ny][nx] = enemy;
                    current_level->level[y][x] = ' ';
                }
            }
        }
    
    if (enemyNTick >= enemy_n_delay) enemyNTick = 0;
    if (enemySTick >= enemy_s_delay) enemySTick = 0;

    int idx = current_level->index;

    // se il livello non è ancora stato clearato
    if (!levelCleared[idx] && countEnemies(current_level) == 0) {

        levelCleared[idx] = true;   // questo livello è clear
        p.setLives(3);

        // avanti se non ultimo livello
        if (idx < 4) {
            current_level = current_level->next;
            lvl = current_level->index;
            p.setPosition(1, 2);
        }
        bool allCleared = true;
        for (int i = 0; i < 5; i++) {
            if (!levelCleared[i]) {
                allCleared = false;
                break;
            }
        }

        if (allCleared) {
            victory = true;
            ingame = false;
            break;
        }

    }

        // invincibilità
        if (invincible) {
            blinkTick++;
                
            if (blinkTick >= blink_delay) {
                blinkCounter++;
                blinkTick = 0;
            }
        
            if (blinkCounter >= maxBlink) {
                invincible = false;
                blinkCounter = 0;
                blinkTick = 0;
            }
        }

        // render
        werase(screen);
        box(screen, 0, 0);

        mvwprintw(screen, 24, 3,  "Punti: %d", p.getScore());
        mvwprintw(screen, 24, 16, "Vite: %d",  p.getLives());
        mvwprintw(screen, 24, 29, "Tempo: %d", (int)time_left);
        mvwprintw(screen, 0, 2, "Livello: %d", lvl + 1);
        if (levelCleared[current_level->index]) {
            mvwprintw(screen, 0, 14, "[CLEAR]");
        }

        for (int y = 0; y < 22; y++)
            for (int x = 0; x < 42; x++)
                mvwprintw(screen, y + 1, x + 1, "%c",
                          current_level->level[y][x]);

        if (explosionVisible) {
            wattron(screen, COLOR_PAIR(3));   // colore acceso

            for (int i = 0; i < explosionCount; i++) {
                mvwprintw(screen,
                          explosionY[i] + 1,
                          explosionX[i] + 1,
                          "*");
            }
            wattroff(screen, COLOR_PAIR(3));  // colore spento

            if (time(nullptr) - explosionTime >= 1)
                explosionVisible = false;
        }

        if (invincible && (blinkCounter % 2 == 0))
            playerChar = ' '; else playerChar = '@';

        wattron(screen, COLOR_PAIR(2));
        mvwprintw(screen, p.getY() + 1, p.getX() + 1, "%c", playerChar);
        wattroff(screen, COLOR_PAIR(2));

        time_t now = time(nullptr);
        time_left -= (now - start);
        start = now;

        if (time_left < 0) time_left = 0;

        if (p.getLives() <= 0 || time_left <= 0)
            ingame = false;

        wrefresh(screen);
    }

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

    box(prompt, 0, 0);
    mvwprintw(prompt, 1, 2, "Inserisci il tuo nome:");
    wrefresh(prompt);

    wgetnstr(prompt, name, 31);
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