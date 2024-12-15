/*
    File containing function implementation for menu navigation
*/
#include <iostream>
#include "menu.h"
#include "laser_maze.h" // Include the lazergame header
using namespace std;

void showMainMenu(Player& player) {
    int choice;
    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Start Game\n2. View High Scores\n3. Logout\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            startGame(player);
        } else if (choice == 2) {
            showHighScores();
        } else if (choice == 3) {
            ofstream out(player.getUsername() + ".txt");
            out << player; // Save player progress to file
            out.close();
            cout << "\n";
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void startGame(Player& player) {
    int choice;
    cout << "\nSelect Difficulty\n";
    cout << "1. Easy\n2. Medium\n3. Hard\nChoose an option: ";
    cin >> choice;

    int difficultyIndex = -1;
    if (choice == 1) {
        difficultyIndex = 0;
        player.resetLives();
        selectLevel(player, "Easy");
    } else if (choice == 2) {
        difficultyIndex = 1;
        player.resetLives();
        selectLevel(player, "Medium");
    } else if (choice == 3) {
        difficultyIndex = 2;
        player.resetLives();
        selectLevel(player, "Hard");
    } else {
        cout << "Invalid option. Please try again.\n";
        return;
    }
}

void selectLevel(Player& player, const string& difficulty) {
    int choice;
    int difficultyIndex = (difficulty == "Easy") ? 0 : (difficulty == "Medium") ? 1 : 2;

    while (true) {
        cout << "\nSelect Level (" << difficulty << ")\n";
        cout << "1. Level 1\n";
        cout << "2. Level 2\n";
        cout << "Choose an option: ";
        cin >> choice;
        string levelName = difficulty + " Level " + to_string(choice);

        if (choice == 1) {
            cout << "Starting " << difficulty << " Level 1...\n";
            // Load autosave if the level matches the last played level
            player.setCurrentLevel(levelName);
            player.loadAutosave();
            // Load and start the selected level
            if (playGame(player, difficulty, '1')) {
                player.updateProgress(difficultyIndex, choice);
            }
            break;
        } else if (choice == 2) {
            if (player.getProgress(difficultyIndex) >= 1) {
                cout << "Starting " << difficulty << " Level 2...\n";
                // Load autosave if the level matches the last played level
                player.setCurrentLevel(levelName);
                player.loadAutosave();
                // Load and start the selected level
                if (playGame(player, difficulty, '2')) {
                    player.updateProgress(difficultyIndex, choice);
                }
                break;
            } else {
                cout << "You must beat Level 1 first before playing Level 2.\n";
            }
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void showHighScores() {
    cout << "\nHigh Scores\n";
    // Display high scores
}