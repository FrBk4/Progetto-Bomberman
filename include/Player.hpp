#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
private:
    int x;
    int y;
    int lives;
    int score = 0;

public:
    Player(int startX, int startY);

    void move(int dx, int dy);
    void setPosition(int y, int x);
    void addScore(int s);
    void setLives(int l);
    void loseLife();
    
    bool isAtPosition(int otherX, int otherY) const;

    int getX() const;
    int getY() const;
    int getLives() const;
    int getScore() const;

};

#endif