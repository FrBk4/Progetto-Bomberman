#ifndef LEVELS_H
#define LEVELS_H

#include<ncurses/ncurses.h>
struct map{
    char level[23][86][5];
};

map genlevels();

WINDOW* enclose_screen(map);

int change_level(map, WINDOW*, bool, int);
#endif //UNTITLED1_LEVELS_H