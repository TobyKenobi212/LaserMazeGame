#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
private:
    std::string username;
    int lives;
    int score;

public:
    Player(const std::string& name);
    void loseLife();
    void gainLife();
    int getLives() const;
    void addScore(int points);
    int getScore() const;
    std::string getUsername() const;
};

#endif // PLAYER_H
