#include <iostream>
#include "login_manager.h"
#include "menu.h"
#include "player.h"
using namespace std;

int main() {
    LoginManager loginManager;
    User user;
    int choice;

    while (true) {
        cout << "1. Register\n2. Login\n3. Exit\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Register a new user\n";
            cin >> user;
            loginManager.registerUser(user.username, user.password);
        } else if (choice == 2) {
            cout << "Login\n";
            cin >> user;

            if (loginManager.login(user.username, user.password)) {
                cout << "Login successful!\n";
                Player player(user.username);
                showMainMenu(player);
            } else {
                cout << "Login failed. Please check your username and password.\n";
            }
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}