/*
    File containing menu function prototypes
*/
#ifndef MENU_H
#define MENU_H

#include <string>
#include "player.h"
#include "leaderboard.h"

void showMainMenu(Player& player, Leaderboard& leaderboard);
void startGame(Player& player);
void showLeaderboardMenu(Leaderboard& leaderboard);
void selectLevel(Player& player, const std::string& difficulty);

#endif // MENU_H