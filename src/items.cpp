#include <iostream>
#include <ctime>
#include <cstring>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Items.hpp"
#include "player.hpp"

using namespace std;

void Itemlist :: effect_list(char effect, map* level, WINDOW* screen, time_t start,
                              time_t* time_effect, int* radius, bool *invincible, Player* p, int* mult, bool *updradius, bool affected[2][5]) {
    switch (effect) {

        case 'b':  //incrementatore di raggio - 10s
            if (!*updradius) {
                if (*radius < 3) {
                    if (tempRadius == true) *radius = 3;
                    else *radius = 2;
                }
                *time_effect = time(nullptr) + 10;
            }
            break;

        case 'm':  //medikit - ripristina una vita
            if (p->getLives() < 3) p->setLives(p->getLives()+1);
            break;

        /* case 'w': //wallbreaker (rarissimo) - distrugge tutte le mura distruttibili del livello

            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]=='+') {
                        level->level[y][x]=' ';
                        p->addScore(20);
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                        affected[0][level->index] = true;
                    }
            break; */

        case 'd': //defender: ti rende invincibile per 10 s
                *invincible = true;
                *time_effect = time(nullptr) + 10;
            break;

        case 'k': //killer, rarissimo: uccide tutti i nemici del livello corrente (clearer del livello)
            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if ((level->level[y][x]>='A' && level->level[y][x]<='Z')||level->level[y][x]=='^') {
                        if (level->level[y][x]=='N') p->addScore(100);
                        if (level->level[y][x]=='S') p->addScore(150);
                        if (level->level[y][x] == 'U')  p->addScore(200 * *mult);
                        if (level->level[y][x] == 'T')  p->addScore(100 * *mult);
                        if (level->level[y][x] == '^')  p->addScore(50 * *mult);
                        level->level[y][x]=' ';
                        mvwprintw(screen, y+1, x+1, " ");
                        affected[1][level->index] = true;
                        wrefresh(screen);
                    }
            break;

        case 'e': //EXP multiplier: per 10 secondi guadagni punti doppi
                *mult = 2;
                *time_effect = time(nullptr) +10;

        case 'r': //permanent radius
            if (*radius < 3) {
                *radius += 1;
                tempRadius = true;
            }
            if (*radius == 3)
                *updradius = true;
            break;

    }
}

char Itemlist :: spawnrate(int prob) { //funzione che genera item casualmente sulla base di un valore probabilistico

    srand(time(nullptr));

    int q = rand() % 100;

    if (q<prob){
       int p = rand() % 100;

       if(p<35) //radius: prob 35%
            return 'b';
        if (p<55) //medikit: prob 20%
            return 'm';
        if (p<75) //defender: prob 20%
            return 'd';
        if (p<95) //EXP multiplier: prob 20%
            return 'e';
        if (p<98) //permanent radius: prob. 3%
            return 'r';
        if (p<100) //killer: prob 2%
            return 'k';
        /*if (p == 99) //wallbreaker: prob 1%
            return 'w';*/

    }return ' ';

}

void Itemlist :: hideitems(map* level, WINDOW* screen) { //nasconde la natura degli item del livello corrente
    for (int y = 0; y < 23; y++)
        for (int x = 0; x < 43; x++)
            if (level->level[y][x]>='a' && level->level[y][x]<='z')
              mvwprintw(screen, y+1, x+1, "?");
}

void Itemlist :: reseteffects(WINDOW* screen, int *radius, bool* invincible, int* mult, bool* updradius) {

    if (!*updradius) *radius = 1;
    *invincible = false;
    *mult = 1;
    box(screen, 0, 0);
    wrefresh(screen);
    tempRadius = false;
}

void Itemlist :: printeffects(WINDOW* screen, char effect, bool w, bool k, bool r) {
    switch (effect) {

        case 'b':  //incrementatore di raggio - 10s
            mvwprintw(screen, 0, 21, "<RADIUS>");
            break;

        case 'm':  //medikit - ripristina una vita
            mvwprintw(screen, 0, 21, "<MEDIKIT>");
            break;



        case 'd': //defender: ti rende invincibile per 10 s
            mvwprintw(screen, 0, 21, "<INVINC.>");
            break;



        case 'e': //EXP multiplier: per 10 secondi guadagni punti doppi
            mvwprintw(screen, 0, 21, "<PUNTIx2>");
            break;

        case 'r': //permanent radius
            mvwprintw(screen, 0, 31, "[RADIUS PERM]");
            break;

    }

    /* if (w)
        mvwprintw(screen, 0, 40, "[W]");*/


    if (k)
        mvwprintw(screen, 0, 35, "[KILL]");
}