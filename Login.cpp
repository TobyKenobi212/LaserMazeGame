#include "Login.h"
using namespace std;

LoginManager::LoginManager() {
    // users for testing
    userDatabase.push_back({"user1", "password1"});
    userDatabase.push_back({"user2", "password2"});
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
    userDatabase.push_back({username, password});
}

istream& operator>>(istream& in, User& user) {
    cout << "Enter username: ";
    is >> user.username;
    cout << "Enter password: ";
    is >> user.password;
    return is;
}

ostream& operator<<(ostream& out, const User& user) {
    os << "Username: " << user.username << ", Password: " << user.password;
    return os;
}