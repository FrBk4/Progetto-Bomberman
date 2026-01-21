#include<iostream>
#include<ctime>
#include "levels.h"
#include<curses.h>
#include "Player.hpp"

using namespace std;


map* Levels::genlevels() {  //questa funzione genera i 5 livelli e ritorna un array che li contiene come matrici 23x86

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
        int prob = (node->index+1)*10;
        for (int y=0; y<23; y++)
            for(int x=0; x<43; x++) {
                if (node->level[y][x]==' ' && (x>5 || y>5)) {
                    int p = rand()%100;
                    if (p<=prob)
                        node->level[y][x]=char(177);
                }
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

    wborder(screen,  186, 186,
                     205, 205,
                     201, 187,
                     200, 188);

    mvwprintw(screen, 24, 22, "Tempo rimasto: %ds", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",0); //si suppone sia 0 per ovvie ragioni a inizio game, meglio che passare un'altra variabile

    wrefresh(screen);

    return screen;
}


//



map* Levels::change_level(map *head, WINDOW* screen, bool action, int lvl, int time_left, int points) {

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

    wborder(screen,  186, 186,
                   205, 205,
                   201, 187,
                   200, 188);
    mvwprintw(screen, 24, 22, "Tempo rimasto: %d s", time_left);
    mvwprintw(screen, 24, 3, "Punti: %d",points);

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
    int points = 0;
    Map = genlevels();  //funzioni di generazione della mappa
    WINDOW* screen = enclose_screen(Map, (int)time_left);
    map* current_level = Map;
    keypad(screen, true);
    halfdelay(2);
    int lvl = 0;

    Player p(1, 1);
    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
    wrefresh(screen);

    bool ingame = true;
    while (ingame == true) {    //input loop

        int move = wgetch(screen);

        bool scored_point = false;
        switch (move) {
            case 'w':
            case 'W':
            case KEY_UP:
                if (p.getY()>1 && current_level->level[p.getY()-1][p.getX()] != char(219) && current_level->level[p.getY()-1][p.getX()] != char(177)) {
                    p.move(-1, 0);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+2, p.getX()+1, "%c", current_level->level[p.getY()+1][p.getX()]);
                }
                break;

            case 'a':
            case 'A':
            case KEY_LEFT:
                if (/*p.getX()>2 &&*/ current_level->level[p.getY()][p.getX()-1] != char(219) && current_level->level[p.getY()][p.getX()-1] != char(177)) {
                    p.move(0, -1);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+1, p.getX()+2, "%c", current_level->level[p.getY()][p.getX()+1]);
                }
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if (/*p.getY()<22 &&*/ current_level->level[p.getY()+1][p.getX()] != char(219) && current_level->level[p.getY()+1][p.getX()] != char(177)) {
                    p.move(1, 0);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY(), p.getX()+1, "%c", current_level->level[p.getY()-1][p.getX()] );
                }
                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (/*p.getX()<84 &&*/ current_level->level[p.getY()][p.getX()+1] != char(219) && current_level->level[p.getY()][p.getX()+1] != char(177)) {
                    p.move(0, 1);
                    mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
                    mvwprintw(screen, p.getY()+1, p.getX(), "%c", current_level->level[p.getY()][p.getX()-1]);
                }
                break;

            case 27: //ESC
                ingame = false;
                break;

            /*case 'p':
            case 'P':
                int x_offset = getmaxx(stdscr) / 2 - 43;
                if (x_offset < 0) x_offset = 0;
                WINDOW* controls = newwin(23, 86, 3, x_offset);
                mvwprintw(controls,1,1, "> Comandi del gioco: < \n\n");
                wprintw(controls, " > WASD / keypad: movimento \n> E: piazza mina \n> ESC: esci \n\n Premi P o ESC per uscire.");
                box(controls, 0, 0);
                do {wrefresh(controls);}while(getch() != 27 && getch()!= 'P');
                delwin(controls);
                refresh();*/
        }

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
            current_level = change_level(Map, screen, action, lvl, time_left, points);
            mvwprintw(screen, p.getY()+1, p.getX()+1, "%c", p.getSymbol());
        }

        time_t finish = time(nullptr);
        time_left -= (finish - start);
        if (finish != start) start = finish;
        if (time_left==1000) mvwprintw(screen, 24, 22, "Tempo rimasto: %ds", (int)time_left);
        else mvwprintw(screen, 24, 22, "Tempo rimasto: %d ", (int)time_left);
        if (time_left <= 0) ingame = false;
        if (scored_point == true) points++;
        mvwprintw(screen, 24, 3, "Punti: %d",points);
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



