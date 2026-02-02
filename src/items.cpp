#include <iostream>
#include <ctime>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Items.hpp"
#include "player.hpp"

using namespace std;

void Itemlist :: effect_list(char effect, int* player_lives, map* level, WINDOW* screen, time_t start, time_t* time_effect, int* radius, bool *invincible, Player* p, int* mult) {
    switch (effect) {

        case 'B':  //incrementatore di raggio - 10s
            if ( *time_effect == 0) {
                if (*radius < 2) *radius+=1;
                *time_effect = time(nullptr) + 10;
            }
            break;

        case 'M':  //medikit - ripristina una vita
            if (*player_lives < 3) *player_lives += 1;
            break;

        case 'W': //wallbreaker (rarissimo) - distrugge tutte le mura distruttibili del livello

            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]=='+') {
                        level->level[y][x]=' ';
                        p->addScore(20);
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                    }
            break;

        case 'D': //defender: ti rende invincibile per 10 s
            if ( *time_effect == 0) {
                *invincible = true;
                *time_effect = time(nullptr) + 10;
            }
            break;

        case 'K': //killer, rarissimo: uccide tutti i nemici del livello corrente (clearer del livello)
            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]=='N' || level->level[y][x]=='S') {
                        if (level->level[y][x]=='N') p->addScore(100);
                        if (level->level[y][x]=='S') p->addScore(150);
                        level->level[y][x]=' ';
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                    }
            break;

        case 'E': //EXP multiplier: per 10 secondi guadagni punti doppi
            if ( *time_effect == 0) {
                *mult = 2;
                *time_effect = time(nullptr) +10;
            }


    }
}

char Itemlist :: spawnrate(int prob) { //funzione che genera item casualmente sulla base di un valore probabilistico

    int q = rand() % 100;

    if (q<prob){
       int p = rand() % 100;

        if (p<38) //radius: prob 38%
            return 'B';
        if (p<58) //medikit: prob 20%
            return 'M';
        if (p<78) //defender: prob 20%
            return 'D';
        if (p<98) //EXP multiplier: prob 20%
            return 'E';
        if (p == 98) //killer: prob 1%
            return 'K';
        if (p == 99) //wallbreaker: prob 1%
            return 'W';

    }return ' ';

}

void Itemlist :: hideitems(map* level, WINDOW* screen) { //nasconde la natura degli item del livello corrente
    for (int y = 0; y < 23; y++)
        for (int x = 0; x < 43; x++)
            if (level->level[y][x]=='B' ||level->level[y][x]=='M' ||level->level[y][x]=='W' ||
                level->level[y][x]=='D' ||level->level[y][x]=='K' ||level->level[y][x]=='E')
                mvwprintw(screen, y+1, x+1, "?");
}

void Itemlist :: reseteffects(int *radius, bool* invincible, int* mult) {

    *radius = 1;
    *invincible = false;
    *mult = 1;
}
