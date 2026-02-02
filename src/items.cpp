#include <iostream>
#include <ctime>
#include <curses.h>
#include "../include/Levels.hpp"
#include "../include/Items.hpp"

using namespace std;

void Itemlist :: effect_list(char effect, int* player_lives, map* level, WINDOW* screen, time_t start, time_t* time_effect, int* radius) {
    switch (effect) {

        case 'B':  //incrementatore di raggio - 10s
            *radius+=1;
            *time_effect = time(nullptr) + 10;
            break;

        case 'M':  //medikit - ripristina le vite
            *player_lives = 3;
            break;

        case 'W': //wallbreaker (rarissimo) - distrugge tutte le mura distruttibili del livello

            for (int y = 0; y< 23; y++)
                for (int x = 0; x < 43; x++)
                    if (level->level[y][x]==char(177)) {
                        level->level[y][x]=' ';
                        mvwprintw(screen, y+1, x+1, " ");
                        wrefresh(screen);
                    }
            break;
    }
}

char Itemlist :: spawnrate(int prob) { //funzione che genera item casualmente sulla base di un valore probabilistico

    int q = rand() % 100;

    if (q<prob){
       int p = rand() % 1000;

        if (p<800)
            return 'B';
        if (p<999)
            return 'M';

            return 'W';

    }return ' ';

}

void Itemlist :: hideitems(map* level, WINDOW* screen) { //nasconde la natura degli item del livello corrente
    for (int y = 0; y < 23; y++)
        for (int x = 0; x < 43; x++)
            if (level->level[y][x]=='B' ||level->level[y][x]=='M' ||level->level[y][x]=='W')
                mvwprintw(screen, y+1, x+1, "?");
}

void Itemlist :: reseteffects(int *radius) {

    *radius = 1;
}