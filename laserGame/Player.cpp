/*
    File containing class method implementation for Player.h
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

// Autosave function
void Player::autosave() {
    ofstream outFile("autosaves/" + username + "_autosave.txt");
    if (outFile.is_open()) {
        outFile << username << endl;
        outFile << lives << endl;
        outFile << score << endl;
        for (int i = 0; i < 3; ++i) {
            outFile << progress[i] << endl;
        }
        outFile << currentLevel << endl;
        outFile.close();
        cout << "Progress autosaved to " << username << "_autosave.txt\n";
    } else {
        cout << "Unable to open autosave file!\n";
    }
}

// Load the autosave if it exists
void Player::loadAutosave() {
    ifstream inFile("autosaves/" + username + "_autosave.txt");
    if (inFile.is_open()) {
        string tempUsername, tempCurrentLevel;
        int tempLives, tempScore, tempProgress[3];
        
        // Read in the exact same order as autosave
        if (getline(inFile, tempUsername) &&
            inFile >> tempLives &&
            inFile >> tempScore) {
            
            for (int i = 0; i < 3; ++i) {
                inFile >> tempProgress[i];
            }
            
            // Consume newline before reading currentLevel
            string dummy;
            getline(inFile, dummy);
            
            getline(inFile, tempCurrentLevel);
            // Verify autosave matches current game state
            if (tempUsername == username && tempCurrentLevel == currentLevel) {
                lives = tempLives;
                score = tempScore;
                for (int i = 0; i < 3; ++i) {
                    progress[i] = tempProgress[i];
                }
                
                cout << "Autosave loaded for level: " << currentLevel << "\n";
            } else {
                remove(("autosaves/" + username + "_autosave.txt").c_str());
                cout << "Autosave does not match current level. Starting fresh.\n";
            }
        } else {
            cout << "Error reading autosave file.\n";
            remove(("autosaves/" + username + "_autosave.txt").c_str());
        }
        
        inFile.close();
    } else {
        cout << "No autosave found. Starting fresh.\n";
    }
}

// Sets the current level the player is on
void Player::setCurrentLevel(const string& level) {
    currentLevel = level;
}

// Returns the current level the player is on
string Player::getCurrentLevel() const {
    return currentLevel;
}

// Overloaded output stream operator: Writes player data to an output stream
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