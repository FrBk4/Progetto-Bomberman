#ifndef ITEMS_H
#define ITEMS_H

#include <ctime>
#include "player.hpp"

class Itemlist {
    bool tempRadius = false;
public:
    char spawnrate(int prob);
    void effect_list(char effect, map* level, WINDOW* screen, time_t start,
                     time_t* time_effect, int* radius, bool *invincible, Player* p, int* mult, bool* updradius, bool affected[2][5]);
    void hideitems(map* level, WINDOW* screen);
    void reseteffects(WINDOW* screen, int* radius, bool* invincible, int* mult, bool *updradius);
    void printeffects(WINDOW* screen, char effect, bool w, bool k, bool r);
};

#endif