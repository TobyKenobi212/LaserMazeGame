#include <iostream>
#include "menu.h"
#include "laser_maze.h"
using namespace std;

void showMainMenu(Player& player) {
    int choice;
    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Start Game\n2. View High Scores\n3. Log Out\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            startGame(player);
        } else if (choice == 2) {
            showHighScores();
        } else if (choice == 3) {
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

    if (choice == 1) {
        selectLevel(player, "Easy");
    } else if (choice == 2) {
        selectLevel(player, "Medium");
    } else if (choice == 3) {
        selectLevel(player, "Hard");
    } else {
        cout << "Invalid option. Please try again.\n";
    }
}

void selectLevel(const string& difficulty) {
    char choice;
    int difficultyIndex = (difficulty == "Easy") ? 0 : (difficulty == "Medium") ? 1 : 2;

    cout << "\nSelect Level (" << difficulty << ")\n";
    cout << "1. Level 1\n";
    if (player.getProgress(difficultyIndex) >= 1) {
        cout << "2. Level 2\n";
    }
    cout << "Choose an option: ";
    cin >> choice;

    if (choice == '1') {
        cout << "Starting " << difficulty << " Level 1...\n";
        // Load and start the selected level
        playGame(difficulty, choice);
        player.updateProgress(difficultyIndex, 1);

    } else if (choice == '2') {
        cout << "Starting " << difficulty << " Level 2...\n";
        // Load and start the selected level
        playGame(difficulty, choice);
        player.updateProgress(difficultyIndex, 2);

    } else {
        cout << "Invalid option. Please try again.\n";
    }
}

void showHighScores() {
    cout << "\nHigh Scores\n";
    // Display high scores
}