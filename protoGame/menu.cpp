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

    if (choice == 1) {
        player.resetLives();
        selectLevel(player, "Easy");
    } else if (choice == 2) {
        player.resetLives();
        selectLevel(player, "Medium");
    } else if (choice == 3) {
        player.resetLives();
        selectLevel(player, "Hard");
    } else {
        cout << "Invalid option. Please try again.\n";
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

        if (choice == 1) {
            cout << "Starting " << difficulty << " Level 1...\n";
            // Load and start the selected level
            if (playGame(player, difficulty, '1')) {
                player.updateProgress(difficultyIndex, 1);
            }
            break;
        } else if (choice == 2) {
            if (player.getProgress(difficultyIndex) >= 1) {
                cout << "Starting " << difficulty << " Level 2...\n";
                // Load and start the selected level
                if (playGame(player, difficulty, '2')) {
                    player.updateProgress(difficultyIndex, 2);
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