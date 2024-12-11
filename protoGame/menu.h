#ifndef MENU_H
#define MENU_H

#include <string>
#include "player.h"

void showMainMenu(Player& player);
void startGame(Player& player);
void showHighScores();
void selectLevel(Player& player, const std::string& difficulty);

#endif // MENU_H