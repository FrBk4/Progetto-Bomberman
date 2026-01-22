#include "../include/Player.hpp"

Player::Player(int startY, int startX) {
    x = startX;
    y = startY;
    lives = 3;
    score = 0;
    symbol = '@';
}

void Player::move(int dy, int dx) {
    x += dx;
    y += dy;
}

void Player::setposition(int py, int px) {
    y = py;
    x = px;
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

int Player::setLives(int l) {
    lives = l;
}

int Player::getradius() {
    return bombradius;
}

void Player::setradius(int radius) {
    bombradius = radius;
}
