#ifndef ASCII_BOMBERMAN_MENU_H
#define ASCII_BOMBERMAN_MENU_H

#include <string>

class Menu {
private:
    static const int LogoLines = 6;
    static const int LOGO_MAXW  = 140;

    char logoGrid[LogoLines][LOGO_MAXW];
    int flameTicks[LogoLines][LOGO_MAXW];
    int logoWidth = 0;
    bool logoReady = false;

    double startTime = -1.0;

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
    void draw(const std::string items[],
              int countItems,
              int selected,
              int key,
              const std::string subtitles[],
              int countSubt);
public:
    int run(const std::string items[],
              int countItems,
              const std::string subtitles[],
              int countSubt);
};

#endif // ASCII_BOMBERMAN_MENU_H
