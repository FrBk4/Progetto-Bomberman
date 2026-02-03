#include <iostream>
#include <ctime>
#include <cstring>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Items.hpp"
#include "player.hpp"

using namespace std;

void Itemlist :: effect_list(char effect, int* player_lives, map* level, WINDOW* screen, time_t start,
                              time_t* time_effect, int* radius, bool *invincible, Player* p, int* mult, bool *updradius) {
    switch (effect) {

        case 'b':  //incrementatore di raggio - 10s
            if ( *time_effect == 0 && !*updradius) {
                if (*radius < 2) *radius+=1;
                *time_effect = time(nullptr) + 10;
            }
            break;

        case 'm':  //medikit - ripristina una vita
            if (*player_lives < 3) *player_lives += 1;
            break;

        case 'w': //wallbreaker (rarissimo) - distrugge tutte le mura distruttibili del livello

            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]=='+') {
                        level->level[y][x]=' ';
                        p->addScore(20);
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                    }
            break;

        case 'd': //defender: ti rende invincibile per 10 s
            if ( *time_effect == 0) {
                *invincible = true;
                *time_effect = time(nullptr) + 10;
            }
            break;

        case 'k': //killer, rarissimo: uccide tutti i nemici del livello corrente (clearer del livello)
            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]=='N' || level->level[y][x]=='S') {
                        if (level->level[y][x]=='N') p->addScore(100);
                        if (level->level[y][x]=='S') p->addScore(150);
                        if (level->level[y][x] == 'U')  p->addScore(200 * *mult);
                        if (level->level[y][x] == 'T')  p->addScore(100 * *mult);
                        if (level->level[y][x] == '^')  p->addScore(50 * *mult);
                        level->level[y][x]=' ';
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                    }
            break;

        case 'e': //EXP multiplier: per 10 secondi guadagni punti doppi
            if ( *time_effect == 0) {
                *mult = 2;
                *time_effect = time(nullptr) +10;
            }

        case 'r': //permanent radius
           *radius = 2;
            *updradius = true;
            break;

    }
}

char Itemlist :: spawnrate(int prob) { //funzione che genera item casualmente sulla base di un valore probabilistico

    int q = rand() % 100;

    if (q<prob){
       int p = rand() % 100;

        if (p<33) //radius: prob 33%
            return 'b';
        if (p<53) //medikit: prob 20%
            return 'm';
        if (p<73) //defender: prob 20%
            return 'd';
        if (p<93) //EXP multiplier: prob 20%
            return 'e';
        if (p<97) //permanent radius: prob. 5%
            return 'r';
        if (p == 98) //killer: prob 1%
            return 'k';
        if (p == 99) //wallbreaker: prob 1%
            return 'w';

    }return ' ';

}

void Itemlist :: hideitems(map* level, WINDOW* screen) { //nasconde la natura degli item del livello corrente
    for (int y = 0; y < 23; y++)
        for (int x = 0; x < 43; x++)
            if (level->level[y][x]>='a' && level->level[y][x]<='z' )
                mvwprintw(screen, y+1, x+1, "?");
}

void Itemlist :: reseteffects(int *radius, bool* invincible, int* mult, bool* updradius) {

    if (!*updradius) *radius = 1;
    *invincible = false;
    *mult = 1;
}