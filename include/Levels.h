#ifndef LEVELS_H
#define LEVELS_H

#include<curses.h>

struct map{
    char level[23][86][5];
};

class Levels {
    map Map;
public:
    map genlevels();

    WINDOW* enclose_screen(map*);

    int change_level(map*, WINDOW*, bool, int);

    void run();
};


#endif //UNTITLED1_LEVELS_H