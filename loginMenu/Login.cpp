#include "Login.h"
using namespace std;

LoginManager::LoginManager() {
    // Pre-populate with some users for testing
    userDatabase.push_back({"Edward", "password935"});
    userDatabase.push_back({"Ludvig", "password115"});
}

bool LoginManager::login(const string& username, const string& password) {
    for (const auto& user : userDatabase) {
        if (user.username == username && user.password == password) {
            return true;
        }
    }
    return false;
}

void LoginManager::registerUser(const string& username, const string& password) {
    if (!userExists(username)) {
        userDatabase.push_back({username, password});
    } else {
        cout << "Username already exists. Please choose a different username.\n";
    }
}

bool LoginManager::userExists(const string& username) const {
    for (const auto& user : userDatabase) {
        if (user.username == username) {
            return true;
        }
    }
    return false;
}

istream& operator>>(istream& is, User& user) {
    cout << "Enter username: ";
    is >> user.username;
    cout << "Enter password: ";
    is >> user.password;
    return is;
}

ostream& operator<<(ostream& os, const User& user) {
    os << "Username: " << user.username << ", Password: " << user.password;
    return os;
}