/*
    Header file containing login class and login prototypes.
    Author(s): Toby Pham, Tri Nguyen, Benjamin Thai
    Last updated 12/15/2024
    Version 1.03
*/

#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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

    friend std::ostream& operator<<(std::ostream& out, const LoginManager& manager);
    friend std::istream& operator>>(std::istream& in, LoginManager& manager);
};


#endif // LOGIN_MANAGER_H
