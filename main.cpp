#include <iostream>
#include "Login.h"

using namespace std;

void showMainMenu();
void startGame();
void showHighScores();
void selectLevel(const string& difficulty);

int main() {
    LoginManager loginManager;
    User user;
    int choice;

    while (true) {
        cout << "1. Register\n2. Log in\n3. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Register a new user\n";
            cin >> user;
            loginManager.registerUser(user.username, user.password);
        } else if (choice == 2) {
            cout << "Log in\n";
            cin >> user;

            if (loginManager.login(user.username, user.password)) {
                cout << "Login successful!\n";
                showMainMenu();
            } else {
                cout << "Login failed. Please check your username and password.\n";
            }
        } else if (choice == 3) {
            cout << "Exiting program...";
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}

void showMainMenu() {
    int choice;
    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Start Game\n2. View High Scores\n3. Log Out\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            startGame();
        } else if (choice == 2) {
            showHighScores();
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }
}

void startGame() {
    int choice;
    cout << "\nSelect Difficulty\n";
    cout << "1. Easy\n2. Medium\n3. Hard\nChoose an option: ";
    cin >> choice;

    if (choice == 1) {
        selectLevel("Easy");
    } else if (choice == 2) {
        selectLevel("Medium");
    } else if (choice == 3) {
        selectLevel("Hard");
    } else {
        cout << "Invalid option. Please try again.\n";
    }
}

void selectLevel(const string& difficulty) {
    int choice;
    cout << "\nSelect Level (" << difficulty << ")\n";
    cout << "1. Level 1\n2. Level 2\nChoose an option: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Starting " << difficulty << " Level 1...\n";
        // Load and start the selected level
    } else if (choice == 2) {
        cout << "Starting " << difficulty << " Level 2...\n";
        // Load and start the selected level
    } else {
        cout << "Invalid option. Please try again.\n";
    }
}

void showHighScores() {
    cout << "\nHigh Scores\n";
    // Display high scores
}
