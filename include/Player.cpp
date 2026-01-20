#include "Player.hpp"

Player::Player(int startX, int startY) {
    x = startX;
    y = startY;
    lives = 3;
    score = 0;
    symbol = 'P';
}

void Player::move(int dx, int dy) {
    x += dx;
    y += dy;
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }
char Player::getSymbol() const { return symbol; }

void Player::addScore(int value) {
    score += value;
}

int Player::getScore() const {
    return score;
}

void Player::loseLife() {
    lives--;
}

int Player::getLives() const {
    return lives;
}
