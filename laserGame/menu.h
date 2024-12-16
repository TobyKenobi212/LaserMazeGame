/*
    File containing menu function prototypes
    Author(s): Toby Pham, Tri Nguyen, Benjamin Thai
    Last updated 12/15/2024
    Version 1.03
*/
#ifndef MENU_H
#define MENU_H

#include <string>
#include "player.h"
#include "leaderboard.h"

void showMainMenu(Player& player, Leaderboard& leaderboard);
void startGame(Player& player, Leaderboard& leaderboard);
void showLeaderboardMenu(Leaderboard& leaderboard);
void selectLevel(Player& player, const std::string& difficulty, Leaderboard& leaderboard);

#endif // MENU_H
