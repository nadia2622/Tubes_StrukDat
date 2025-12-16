#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

struct User
{
    int id;
    string username;
    string password;
    string role;

    User() : id(0), username(""), password(""), role("user") {}

    User(int id, string username, string password, string role)
        : id(id), username(username), password(password), role(role) {}

    void display() const
    {
        cout << "ID: " << id
             << " | Username: " << username
             << " | Role: " << role << endl;
    }
};

class Auth
{
private:
    map<string, User> users;
    int nextUserId;
    User *currentUser;

    string hashPassword(const string &password);
    bool verifyPassword(const string &inputPassword, const string &storedPassword);
    bool usernameExists(const string &username);
    string getPasswordInput(const string &prompt);

public:
    Auth();
    ~Auth();

    bool registerUser(const string &username, const string &password, const string &role = "user");
    bool registerWithMaskedPassword(const string &username, const string &role = "user");
    bool login(const string &username, const string &password);
    bool loginWithMaskedPassword(const string &username);
    void logout();
    bool isLoggedIn() const;

    User *getCurrentUser();
    bool isAdmin() const;
    bool isUser() const;

    vector<User> getAllUsers() const;
    bool deleteUser(const string &username);
    bool updateUserRole(const string &username, const string &newRole);

    void loadDefaultUsers();
    int getUserCount() const;
    void displayCurrentUser() const;
    void displayAllUsers() const;
};

#endif