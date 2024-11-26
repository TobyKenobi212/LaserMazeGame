#include "Login.h"
using namespace std;

LoginManager::LoginManager() {
    // Pre-populate with some users for testing
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
