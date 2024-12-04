#include <iostream>
#include "login_manager.h"
using namespace std;

int main() {
    LoginManager loginManager;
    User user;

    cout << "Register a new user\n";
    cin >> user;
    loginManager.registerUser(user.username, user.password);

    cout << "\nLog in\n";
    cin >> user;

    if (loginManager.login(user.username, user.password)) {
        cout << "Login successful!\n";
    } else {
        cout << "Login failed. Please check your username and password.\n";
    }

    return 0;
}