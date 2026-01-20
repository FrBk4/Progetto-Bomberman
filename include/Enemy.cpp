#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(int startX, int startY, int spd, char sym) {
    x = startX;
    y = startY;
    speed = spd;
    symbol = sym;
    alive = true;
}

void Enemy::moveRandom() {
    int dir = rand() % 4;
    if (dir == 0) x += speed;
    if (dir == 1) x -= speed;
    if (dir == 2) y += speed;
    if (dir == 3) y -= speed;
}

void Enemy::kill() {
    alive = false;
}

int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
char Enemy::getSymbol() const { return symbol; }
bool Enemy::isAlive() const { return alive; }
