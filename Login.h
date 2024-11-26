#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>

struct User {
    std::string username;
    std::string password;
};

class LoginManager {
private:
    std::vector<User> userDatabase; // Vector to store users

public:
    LoginManager();
    bool login(const std::string& username, const std::string& password);
    void registerUser(const std::string& username, const std::string& password);
};

#endif // LOGIN_MANAGER_H
