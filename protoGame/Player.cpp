/*
    File containing class method implementation for laser_maze.h
*/
#include "player.h"
using namespace std;

// Constructor: Initializes a new Player with a username, 3 lives, and zero score
Player::Player(const string& name) : username(name), lives(3), score(0) {
    progress[0] = 0; // Easy
    progress[1] = 0; // Medium
    progress[2] = 0; // Hard
}

// Reduces the player's lives by 1 if they have lives remaining
void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

// Checks if the player has run out of lives
bool Player::isOutOfLives() const {
    return lives <= 0;
}

// Resets the player's lives back to 3
void Player::resetLives() {
    lives = 3;
}   

// Returns the current number of lives the player has
int Player::getLives() const {
    return lives;
}

// Prints a visual representation of the player's remaining lives
void Player::printLives() const {
    cout << "Lives: ";
    for (int i = 0; i < lives; ++i) {
        cout << "| ";
    }
    cout << endl;
}

// Adds points to the player's current score
void Player::addScore(int points) {
    score += points;
}

// Returns the player's current score
int Player::getScore() const {
    return score;
}

// Returns the player's username
string Player::getUsername() const {
    return username;
}

// Retrieves the player's progress for a specific difficulty level
int Player::getProgress(int difficulty) const {
    return progress[difficulty];
}

// Updates the player's progress for a specific difficulty level if the new level is higher
void Player::updateProgress(int difficulty, int level) {
    if (level > progress[difficulty]) {
        progress[difficulty] = level;
    }
}

// Autosave function
void Player::autosave() {
    ofstream outFile(username + "_autosave.txt");
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
    ifstream inFile(username + "_autosave.txt");
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
                remove((username + "_autosave.txt").c_str());
                cout << "Autosave does not match current level. Starting fresh.\n";
            }
        } else {
            cout << "Error reading autosave file.\n";
            remove((username + "_autosave.txt").c_str());
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
    }
    return out;
}

// Overloaded input stream operator: Reads player data from an input stream
istream& operator>>(istream& in, Player& player) {
    in >> player.username;
    in >> player.lives;
    in >> player.score;
    for (int i = 0; i < 3; ++i) {
        in >> player.progress[i];
    }
    return in;
}