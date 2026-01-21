#ifndef LEVELS_H
#define LEVELS_H

#include<curses.h>

struct map{
    char level[23][86];
    int index;
    map* previous;
    map* next;
};

class Levels {
    map* Map;
public:
    map* genlevels();

    WINDOW* enclose_screen(map*);

    map* change_level(map*, WINDOW*, bool, int);

    void run();
};


#endif //UNTITLED1_LEVELS_H