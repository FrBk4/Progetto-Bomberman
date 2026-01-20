#ifndef ASCII_BOMBERMAN_GAME_H
#define ASCII_BOMBERMAN_GAME_H

#endif //ASCII_BOMBERMAN_GAME_H

#include "Menu.h"
#include "leaderboard.h"

class Game {
public:
    void run();

private:
    void initCurses();
    void shutdownCurses();

    Menu menu_;
    Leaderboard lb_;
};