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

bool Player::isOutOfLives() const {
    return lives <= 0;
}

void Player::resetLives() {
    lives = 3;
}   

int Player::getLives() const {
    return lives;
}

void Player::printLives() const {
    cout << "Lives: ";
    for (int i = 0; i < lives; ++i) {
        cout << "| ";
    }
    cout << endl;
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

ostream& operator<<(ostream& out, const Player& player) {
    out << player.username << endl;
    out << player.lives << endl;
    out << player.score << endl;
    for (int i = 0; i < 3; ++i) {
        out << player.progress[i] << endl;
    }
    return out;
}

istream& operator>>(istream& in, Player& player) {
    in >> player.username;
    in >> player.lives;
    in >> player.score;
    for (int i = 0; i < 3; ++i) {
        in >> player.progress[i];
    }
    return in;
}