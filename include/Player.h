#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
private:
    int x;
    int y;
    int lives;
    int score;
    char symbol;
    int bombradius;

public:
    Player(int startY, int startX);

    void move(int dy, int dx);
    void setposition(int py, int px);

    int getX();
    int getY();
    int getLives();
    int getScore();
    char getSymbol();
    int getradius();

    void setLives(int l);
    void addScore(int s);
    void setradius(int radius);
};

#endif