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
    Player(int startY, int startX);

    void move(int dy, int dx);

    int getX() const;
    int getY() const;
    char getSymbol() const;

    void addScore(int value);
    int getScore() const;

    void loseLife();
    int getLives() const;
};

#endif
