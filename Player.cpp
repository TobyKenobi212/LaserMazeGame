#include "player.h"
using namespace std;

Player::Player(const string& name) : username(name), lives(3), score(0) {}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

void Player::gainLife() {
    lives++;
}

int Player::getLives() const {
    return lives;
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() const {
    return score;
}

string Player::getUsername() const {
    return username;
}
