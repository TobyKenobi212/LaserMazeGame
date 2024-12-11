#include "player.h"
using namespace std;

Player::Player(const string& name) : username(name), lives(3), score(0) {
    progress[0] = 0; // Easy
    progress[1] = 0; // Medium
    progress[2] = 0; // Hard
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
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

int Player::getProgress(int difficulty) const {
    return progress[difficulty];
}

void Player::updateProgress(int difficulty, int level) {
    if (level > progress[difficulty]) {
        progress[difficulty] = level;
    }
}