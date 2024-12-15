#include <iostream>
#include "Login.h"
#include "menu.h"
#include "player.h"
#include "leaderboard.h"
using namespace std;

void showMainMenu(Player& player, Leaderboard& leaderboard);

int main() {
    Leaderboard leaderboard;
    LoginManager loginManager;
    ifstream inFile("users.txt");
    if (inFile) {
        inFile >> loginManager; // Load user data from file
        inFile.close();
    }
    User user;
    int choice;

    while (true) {
        cout << "1. Register\n2. Login\n3. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Register a new user\n";
            cin >> user;
            loginManager.registerUser(user.username, user.password);
            Player player(user.username);
            leaderboard.addPlayer(player); // Add new player to leaderboard
        } else if (choice == 2) {
            cout << "\nLogin\n";
            cin >> user;

            if (loginManager.login(user.username, user.password)) {
                cout << "Login successful!\n";
                Player player(user.username);
                if (user.username != "dev") {
                    ifstream playerFile("player_data/" + player.getUsername() + ".txt");
                    if (playerFile) {
                        playerFile >> player; // Load player progress from file
                        playerFile.close();
                    }
                } else {
                    // Reset dev account progress
                    player = Player("dev");
                }
                leaderboard.addPlayer(player); // Add player to leaderboard
                showMainMenu(player, leaderboard);
            } else {
                cout << "Login failed. Please check your username and password.\n";
            }
        } else if (choice == 3) {
            ofstream outFile("users.txt");
            outFile << loginManager; // Save user data to file
            outFile.close();
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}