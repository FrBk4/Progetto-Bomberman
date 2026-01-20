#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
private:
    int x;
    int y;
    int lives;
    int score;
    char symbol;

public:
    Player(int startX, int startY);

    void move(int dx, int dy);

    int getX() const;
    int getY() const;
    char getSymbol() const;

    void addScore(int value);
    int getScore() const;

    void loseLife();
    int getLives() const;
};

#endif
