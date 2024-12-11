#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
private:
    std::string username;
    int lives;
    int score;
    int progress[3]; // Progress for Easy, Medium, Hard (0: not started, 1: level 1 completed)

public:
    Player(const std::string& name);
    void loseLife();
    int getLives() const;
    void addScore(int points);
    int getScore() const;
    std::string getUsername() const;
    int getProgress(int difficulty) const;
    void updateProgress(int difficulty, int level);
};

#endif // PLAYER_H