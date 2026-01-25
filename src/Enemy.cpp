#include "../include/Enemy.h"
#include <cstdlib>

Enemy::Enemy(int startY, int startX, int spd, char sym) {
    y = startY;
    x = startX;
    speed = spd;
    symbol = sym;
    alive = true;
}

void Enemy::moveRandom() {
    int r = rand() % 4;
    if (r == 0) y -= speed;
    if (r == 1) y += speed;
    if (r == 2) x -= speed;
    if (r == 3) x += speed;
}

void Enemy::kill() {
    alive = false;
}

int Enemy::getX() {
    return x;
}

int Enemy::getY() {
    return y;
}

char Enemy::getSymbol() {
    return symbol;
}

bool Enemy::isAlive() {
    return alive;
}
