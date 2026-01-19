#ifndef ASCII_BOMBERMAN_MENU_H
#define ASCII_BOMBERMAN_MENU_H

#include <string>

class Menu {
private:
    void draw(const std::string items[],
              int count,
              int selected,
              int key);

    static const int LOGO_LINES = 7;
    static const int LOGO_MAXW  = 140;

    char logoGrid[LOGO_LINES][LOGO_MAXW];
    int flameTicks[LOGO_LINES][LOGO_MAXW];
    int logoWidth = 0;
    bool logoReady = false;

    struct Bomb {
        int xRel;
        int yRel;
        double explodeAt;
        bool exploded;
    };

    Bomb bombs[3];
    bool animActive = false;
    double animEndAt = 0.0;

    void initLogo();
    void startBombAnimation();
    void updateBombAnimation();
    double nowSec();
    void destroyCross(int xRel, int yRel, int R);

public:
    int run(const std::string items[],
              int count);
};

#endif // ASCII_BOMBERMAN_MENU_H
