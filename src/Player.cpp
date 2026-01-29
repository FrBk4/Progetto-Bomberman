#include "../include/Player.hpp"

Player::Player(int startX, int startY)
    : x(startX), y(startY), lives(3) {}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

int Player::getLives() const {
    return lives;
}

void Player::move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

void Player::setPosition(int y, int x) {
    this->y = y;
    this->x = x;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int s) {
    score += s;
}

void Player::setLives(int l) {
    lives = l;
}