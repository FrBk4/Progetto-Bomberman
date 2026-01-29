#ifndef ASCII_BOMBERMAN_GAME_H
#define ASCII_BOMBERMAN_GAME_H

#include "Menu.hpp"
#include "Leaderboard.hpp"
#include "Levels.hpp"

class Game {
public:
    void run();

private:
    void initCurses();
    void shutdownCurses();

    Menu menu_;
    Leaderboard lb_;
    Levels levels_;
};

#endif