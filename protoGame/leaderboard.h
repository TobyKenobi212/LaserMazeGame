/*
    File containing class methods for Leaderboard class
*/
#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <iostream>
#include "player.h"

class Leaderboard {
private:
    std::vector<Player> players;

public:
    void addPlayer(const Player& player);
    void displayLeaderboard() const;
};

#endif // LEADERBOARD_H