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

    int getX() const;
    int getY() const;
    char getSymbol() const;

    void addScore(int value);
    int getScore() const;

    void loseLife();
    int getLives() const;
    int setLives(int l);

    int getradius();
    void setradius(int radius);
};

#endif
