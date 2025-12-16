/*
 * Auth.h
 * Authentication & User Management System (Bagian Ryvanda)
 *
 * Fitur:
 * 1. Login & Register
 * 2. Role management (Admin & User)
 * 3. User management
 * 4. Session management
 */

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

    // Verify password
    bool verifyPassword(const string &inputPassword, const string &storedPassword);

    // Check if username already exists
    bool usernameExists(const string &username);

    // Function untuk input password dengan masking (tampilkan *)
    string getPasswordInput(const string &prompt);

public:
    Auth();
    ~Auth();
    bool registerUser(const string &username, const string &password, const string &role = "user");

    bool login(const string &username, const string &password);

    bool loginWithMaskedPassword(const string &username);

    bool registerWithMaskedPassword(const string &username, const string &role = "user");

    void logout();

    bool isLoggedIn() const;

    User *getCurrentUser();

    bool isAdmin() const;

    bool isUser() const;

    vector<User> getAllUsers() const;

    // deleteUser: Hapus user (admin only)
    bool deleteUser(const string &username);

    // updateUserRole: Ubah role user (admin only)
    bool updateUserRole(const string &username, const string &newRole);

    void loadDefaultUsers();

    int getUserCount() const;

    void displayCurrentUser() const;
};

#endif // AUTH_H