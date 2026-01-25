#define ENEMY_HPP

class Enemy {
private:
    int x;
    int y;
    int speed;
    char symbol;
    bool alive;

public:
    Enemy(int startY, int startX, int speed, char symbol);

    void moveRandom();
    void kill();

    int getX();
    int getY();
    char getSymbol();
    bool isAlive();
};
