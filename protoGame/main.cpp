#include <iostream>
#include "Login.h"
#include "menu.h"
#include "player.h"
#include <limits>
using namespace std;

int main() {
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
        if (!(cin >> choice)) {
            // If input fails, clear the error state and ignore the invalid input
            cin.clear();  // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore the rest of the invalid input
            cout << "Invalid option. Please try again.\n";
            continue;  // Skip the rest of the loop and prompt again
        }

        if (choice == 1) {
            cout << "Register a new user\n";
            cin >> user;
            loginManager.registerUser(user.username, user.password);
        } else if (choice == 2) {
            cout << "\nLogin\n";
            cin >> user;

            if (loginManager.login(user.username, user.password)) {
                cout << "Login successful!\n";
                Player player(user.username);
                if (user.username != "dev") {
                    ifstream playerFile(player.getUsername() + ".txt");
                    if (playerFile) {
                        playerFile >> player; // Load player progress from file
                        playerFile.close();
                    }
                } else {
                    // Reset dev account progress
                    player = Player("dev");
                }
                showMainMenu(player);
            }
        } else if (choice == 3) {
            ofstream outFile("users.txt");
            outFile << loginManager; // Save user data to file
            outFile.close();
            break;
        } 
    }

    return 0;
}