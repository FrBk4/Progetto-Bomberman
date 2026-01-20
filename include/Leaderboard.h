#ifndef ASCII_BOMBERMAN_LEADERBOARD_H
#define ASCII_BOMBERMAN_LEADERBOARD_H

#include <curses.h>

class Leaderboard {
protected:
    static const int logoLines = 4;
    int logoWidth = 0;

    WINDOW* boardWin = nullptr;
    bool initBoard = false;

    int lastH = -1;
    int lastW = -1;

    int boardH = 0;
    int boardW = 0;
    int boardY = 0;
    int boardX = 0;

    bool frameInit = false;

    static const int MAX_SCORES = 100;

    struct Entry {
        char name[32];
        int score;
    };

    Entry entries[MAX_SCORES];
    int entryCount = 0;

    void loadFromFile(const char* filename);
    void sortEntries(); // ordina decrescente per score

    void initWindow(int);
    void destroyWindow();
    void clearInner();

    double nowSec();

    void drawStatic();
    void drawDynamic();
    void draw();

public:
    void run();
};

#endif //ASCII_BOMBERMAN_LEADERBOARD_H
