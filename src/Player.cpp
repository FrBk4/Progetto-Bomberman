#include "../include/Player.h"

Player::Player(int startY, int startX) {
    x = startX;
    y = startY;
    lives = 3;
    score = 0;
    symbol = '@';
    bombradius = 1;
}

void Player::move(int dy, int dx) {
    x += dx;
    y += dy;
}

void Player::setposition(int py, int px) {
    y = py;
    x = px;
}

int Player::getX() {
    return x;
}

int Player::getY() {
    return y;
}

int Player::getLives() {
    return lives;
}

int Player::getScore() {
    return score;
}

char Player::getSymbol() {
    return symbol;
}

int Player::getradius() {
    return bombradius;
}

void Player::setLives(int l) {
    lives = l;
}

void Player::addScore(int s) {
    score += s;
}

void Player::setradius(int radius) {
    bombradius = radius;
}
