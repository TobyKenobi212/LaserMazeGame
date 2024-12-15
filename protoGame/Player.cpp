/*
    File containing class method implementation for laser_maze.h
*/
#include "player.h"
using namespace std;

Player::Player(const string& name) : username(name), lives(3), score(0) {
    progress[0] = 0; // Easy
    progress[1] = 0; // Medium
    progress[2] = 0; // Hard
    highestScores[0][0] = 0; // Easy Level 1
    highestScores[0][1] = 0; // Easy Level 2
    highestScores[1][0] = 0; // Medium Level 1
    highestScores[1][1] = 0; // Medium Level 2
    highestScores[2][0] = 0; // Hard Level 1
    highestScores[2][1] = 0; // Hard Level 2
}

int Player::getHighestScore(int difficulty, int level) const {
    return highestScores[difficulty][level];
}

void Player::updateHighestScore(int difficulty, int level, int score) {
    if (score > highestScores[difficulty][level]) {
        highestScores[difficulty][level] = score;
    }
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
        for (int j = 0; j < 2; ++j) {
            out << player.highestScores[i][j] << endl; // Save highest scores
        }
    }
    return out;
}

istream& operator>>(istream& in, Player& player) {
    in >> player.username;
    in >> player.lives;
    in >> player.score;
    for (int i = 0; i < 3; ++i) {
        in >> player.progress[i];
        for (int j = 0; j < 2; ++j) {
            in >> player.highestScores[i][j]; // Load highest scores
        }
    }
    return in;
}