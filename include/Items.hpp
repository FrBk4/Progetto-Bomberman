#ifndef ITEMS_H
#define ITEMS_H

#include <ctime>

class Itemlist {
public:
    char spawnrate(int prob);
    void effect_list(char effect, int* player_lives, map* level, WINDOW* screen, time_t start, time_t* time_effect, int* radius);
    void hideitems(map* level, WINDOW* screen);
};

#endif