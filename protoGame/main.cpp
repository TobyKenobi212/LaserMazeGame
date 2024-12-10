#include <iostream>
#include "Login.h"
#include "menu.h"

using namespace std;

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
            break;
        } else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}