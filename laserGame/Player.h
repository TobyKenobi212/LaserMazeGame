/*
    File containing class methods for Player class
*/
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include <fstream>

class Player {
private:
    std::string username;
    int lives;
    int score;
    int progress[3]; // Progress for Easy, Medium, Hard (0: not started, 1: level 1 completed)
    int highestScores[3][2]; // Highest scores for Easy (2 levels), Medium (2 levels), Hard (2 levels)
    std::string currentLevel; // tracks current level

public:
    Player(const std::string& name);
    void loseLife();
    int getLives() const;
    void addScore(int points);
    int getScore() const;
    std::string getUsername() const;
    int getProgress(int difficulty) const;
    void updateProgress(int difficulty, int level);
    void resetLives(); 
    bool isOutOfLives() const; 
    void printLives() const;
    int getHighestScore(int difficulty, int level) const;
    void updateHighestScore(int difficulty, int level, int score);
    // Autosaving functionality
    void setCurrentLevel(const std::string& level);
    std::string getCurrentLevel() const;
    void autosave(); // New function to autosave progress
    void loadAutosave(); // Load the autosave if it exists

    friend std::ostream& operator<<(std::ostream& out, const Player& player);
    friend std::istream& operator>>(std::istream& in, Player& player);
};

#endif // PLAYER_H