#ifndef ITEMS_H
#define ITEMS_H
#include "levels.h"

class Itemlist {
public:
    void effect_list(char effect, int* player_lives, map* level, WINDOW* screen, time_t start, time_t* time_effect, int* radius);
    char spawnrate(int prob);
    void hideitems(map* levels, WINDOW* screen);
};


#endif