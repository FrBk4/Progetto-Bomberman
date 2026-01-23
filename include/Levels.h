#ifndef LEVELS_H
#define LEVELS_H

#include<curses.h>

struct map{
    char level[23][43];
    int index;
    map* previous;
    map* next;
};

class Levels {
    map* Map;
public:
    map* genlevels();

    WINDOW* enclose_screen(map*, int, int);

    map* change_level(map*, WINDOW*, bool, int, int, int);

    void run();
};


#endif