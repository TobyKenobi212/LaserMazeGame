#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
#include <iostream>

struct User {
    std::string username;
    std::string password;

    friend std::istream& operator>>(std::istream& is, User& user);
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

class LoginManager {
private:
    std::vector<User> userDatabase; // Vector to store users

public:
    LoginManager();
    bool login(const std::string& username, const std::string& password);
    void registerUser(const std::string& username, const std::string& password);
    bool userExists(const std::string& username) const;
};

#endif // LOGIN_MANAGER_H
