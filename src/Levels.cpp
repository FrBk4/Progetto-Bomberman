#include<iostream>
#include<ctime>
#include "levels.h"
#include<curses.h>
#include "Player.hpp"
#include "items.h"

#define DESTR_RATIO (10+(node->index*4))
#define ITEMS_RATIO (node->index+2)

using namespace std;

Itemlist items;
Player p(1, 1);

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
                    node->level[y][x] = char(219);
                else
                    node->level[y][x] = ' ';
                if (x == 0 || y == 0 || x == 42 || y == 22)
                    node->level[y][x] = char(219);
            }

        if (prev)
            prev->next = node;
        else
            head = node;   // primo nodo

        prev = node;
    }

    srand(time(nullptr));     //generazione casuale di mura distruttibili (=char(177)) che dipende dal livello
    for (map* node = head; node; node = node->next) {
        int prob = DESTR_RATIO;
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (node->level[y][x]==' ' && (x>5 || y>5)) {
                    int p = rand()%100;
                    if (p<=prob)
                        node->level[y][x]=char(177);
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
        if (node->index !=0) node->level[1][0] = char(174);
        if (node->index !=4) node->level[21][42] = char(175);
    }

    return head;
}


//


WINDOW* Levels::enclose_screen(map* map, int time_left) {  //questa funzione mostra su schermo la mappa (inizialmente livello 1)

    int x_offset = getmaxx(stdscr) / 2 - 21;
    if (x_offset < 0) x_offset = 0;

    WINDOW * screen = newwin(25, 45, 3, x_offset);

    for (int y=0; y<23; y++)
        for(int x=0; x<43; x++) {
            mvwprintw(screen, y+1, x+1, "%c", map->level[y][x]);
        }

    items.hideitems(map, screen);

    wborder(screen,  186, 186,
                     205, 205,
                     201, 187,
                     200, 188);

    mvwprintw(screen, 24, 29, "Tempo: %ds", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
    mvwprintw(screen, 24, 16, "Vite: %d",p.getLives());

    wrefresh(screen);

    return screen;
}


//



map* Levels::change_level(map *head, WINDOW* screen, bool action, int lvl, int time_left, int lives) {

    if (action && lvl < 4) lvl++;
    else if (!action && lvl>0) lvl--; // ATTENZIONE: si suppone che il primo lvl passato sia 0 dopo la chiamata di enclose_screen

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

    wborder(screen,  186, 186,
                   205, 205,
                   201, 187,
                   200, 188);
    mvwprintw(screen, 24, 29, "Tempo: %d s", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
    mvwprintw(screen, 24, 16, "Vite: %d", lives);

    wrefresh(screen);

    return node;
}


//



void Levels::run() {
    initscr();
    clear();
    refresh();
    time_t start = time(nullptr); //gestione tempo
    time_t time_left = 1000;
    time_t start_effect = 0;
    Map = genlevels();  //funzioni di generazione della mappa
    WINDOW* screen = enclose_screen(Map, (int)time_left);
    map* current_level = Map;
    keypad(screen, true); //impostazioni dell'input
    halfdelay(2);
    int lvl = 0;

    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
    wrefresh(screen);


    //input loop
    bool ingame = true;
    while (ingame == true) {

        int move = wgetch(screen);

        int scored_points = 0;
        bool lost_life = false;
        switch (move) {
            case 'w':
            case 'W':
            case KEY_UP:
                if (/*p.getY()>1 &&*/ current_level->level[p.getY()-1][p.getX()] != char(219) && current_level->level[p.getY()-1][p.getX()] != char(177)) {
                    p.move(-1, 0);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+2, p.getX()+1, "%c", current_level->level[p.getY()+1][p.getX()]);
                }
                break;

            case 'a':
            case 'A':
            case KEY_LEFT:
                if (current_level->level[p.getY()][p.getX()-1] != char(219) && current_level->level[p.getY()][p.getX()-1] != char(177)) {
                    p.move(0, -1);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+1, p.getX()+2, "%c", current_level->level[p.getY()][p.getX()+1]);
                }
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if (current_level->level[p.getY()+1][p.getX()] != char(219) && current_level->level[p.getY()+1][p.getX()] != char(177)) {
                    p.move(1, 0);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY(), p.getX()+1, "%c", current_level->level[p.getY()-1][p.getX()] );
                }
                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (current_level->level[p.getY()][p.getX()+1] != char(219) && current_level->level[p.getY()][p.getX()+1] != char(177)) {
                    p.move(0, 1);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+1, p.getX(), "%c", current_level->level[p.getY()][p.getX()-1]);
                }
                break;

            case 27: //ESC
                ingame = false;
                break;

            case 'p': //visualizzazione comandi
            case 'P':
                int x_offset = getmaxx(stdscr) / 2 - 21;
                if (x_offset < 0) x_offset = 0;
                WINDOW* controls = newwin(25, 45, 3, x_offset);
                mvwprintw(controls,1,1, "> Comandi del gioco: < \n\n");
                wprintw(controls, " > WASD / keypad: movimento \n > E: piazza mina \n > ESC: esci \n\n Premi P o ESC per tornare al gioco.");
                box(controls, 0, 0);
                char c;
                do {
                    c = wgetch(controls);
                }while(c != 27 && c != 'P'&& c!= 'p');
                delwin(controls);
                wrefresh(screen);
                screen=enclose_screen(current_level, (int)time_left);
                mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
        } //fine input loop


        //fetch item
        if (current_level->level[p.getY()][p.getX()]>=65 && current_level->level[p.getY()][p.getX()]<=90) {
            int lives = p.getLives();
            int radius = p.getradius();
            items.effect_list(current_level->level[p.getY()][p.getX()], &lives, current_level, screen, start, &start_effect, &radius);
            p.setLives(lives);
            p.setradius(radius);
            current_level->level[p.getY()][p.getX()] = ' ';
            mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
        }


        //salti di livello
        if (current_level->level[p.getY()][p.getX()]==char(174) || current_level->level[p.getY()][p.getX()]==char(175)) {
            bool action;
            if (current_level->level[p.getY()][p.getX()]==char(174)) {
                action = 0;
                lvl--;
                p.setposition(21, 41);
            }
            else if (current_level->level[p.getY()][p.getX()]==char(175)) {
                action = 1;
                lvl++;
                p.setposition(1, 1);
            }
            current_level = change_level(Map, screen, action, lvl, time_left, p.getLives());
            mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
        }


        //aggiornamento valori tempo, punti, vita e stampa
        time_t finish = time(nullptr);
        time_left -= (finish - start);
        if (finish != start) start = finish;
        if (time_left==1000) mvwprintw(screen, 24, 29, "Tempo: %ds", (int)time_left);
        else mvwprintw(screen, 24, 29, "Tempo: %d ", (int)time_left);
        if (finish-start_effect>=10) {
            p.setradius(1);
        }
        if (time_left <= 0) ingame = false;
        p.addScore(scored_points);
        mvwprintw(screen, 24, 3, "Punti: %d",p.getScore());
        if (lost_life) p.loseLife();
        if (p.getLives() <= 0) ingame = false;
        mvwprintw(screen, 24, 16, "Vite: %d", p.getLives());
    }

    map* tmp;
    while (Map) {
        tmp = Map;
        Map = Map->next;
        delete tmp;
    }

    delwin(screen);
    endwin();
}







