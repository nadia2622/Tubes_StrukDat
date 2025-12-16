/*
 * Auth.cpp
 * Implementasi Authentication System
 * UPDATED: Added password masking functionality
 */

#include "../include/Auth.h"
#include <iostream>
#include <algorithm>
#include <iomanip> // TAMBAH INI untuk setw

// ===== PLATFORM-SPECIFIC INCLUDES =====
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// ===== CONSTRUCTOR =====
Auth::Auth()
{
    nextUserId = 1;
    currentUser = nullptr;
    loadDefaultUsers();
}

// ===== DESTRUCTOR =====
Auth::~Auth() {}

// ===== HASH PASSWORD =====
string Auth::hashPassword(const string &password)
{
    string hashed = password;
    reverse(hashed.begin(), hashed.end());
    hashed = "HASH_" + hashed;
    return hashed;
}

// ===== VERIFY PASSWORD =====
bool Auth::verifyPassword(const string &inputPassword, const string &storedPassword)
{
    string hashedInput = hashPassword(inputPassword);
    return hashedInput == storedPassword;
}

// ===== CHECK USERNAME EXISTS =====
bool Auth::usernameExists(const string &username)
{
    return users.find(username) != users.end();
}

// ===== GET PASSWORD INPUT =====
string Auth::getPasswordInput(const string &prompt)
{
    string password = "";
    char ch;

    cout << prompt;
    cout.flush();

#ifdef _WIN32
    while (true)
    {
        ch = _getch();

        if (ch == '\r' || ch == '\n')
        {
            cout << endl;
            break;
        }
        else if (ch == '\b' || ch == 127)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += ch;
            cout << '*';
            cout.flush();
        }
    }
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true)
    {
        ch = getchar();

        if (ch == '\n' || ch == '\r')
        {
            cout << endl;
            break;
        }
        else if (ch == 127 || ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += ch;
            cout << '*';
            cout.flush();
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    return password;
}

// ===== REGISTER USER =====
bool Auth::registerUser(const string &username, const string &password, const string &role)
{
    if (username.empty())
    {
        cerr << "Username cannot be empty!" << endl;
        return false;
    }

    if (password.empty())
    {
        cerr << "Password cannot be empty!" << endl;
        return false;
    }

    if (username.length() < 3)
    {
        cerr << "Username must be at least 3 characters!" << endl;
        return false;
    }

    if (password.length() < 6)
    {
        cerr << "Password must be at least 6 characters!" << endl;
        return false;
    }

    if (usernameExists(username))
    {
        cerr << "Username '" << username << "' already exists!" << endl;
        return false;
    }

    if (role != "admin" && role != "user")
    {
        cerr << "Invalid role! Must be 'admin' or 'user'" << endl;
        return false;
    }

    string hashedPassword = hashPassword(password);
    User newUser(nextUserId++, username, hashedPassword, role);
    users[username] = newUser;

    cout << "User '" << username << "' registered successfully!" << endl;
    return true;
}

// ===== REGISTER WITH MASKED PASSWORD =====
bool Auth::registerWithMaskedPassword(const string &username, const string &role)
{
    if (username.empty())
    {
        cerr << "Username cannot be empty!" << endl;
        return false;
    }

    if (username.length() < 3)
    {
        cerr << "Username must be at least 3 characters!" << endl;
        return false;
    }

    if (usernameExists(username))
    {
        cerr << "Username '" << username << "' already exists!" << endl;
        return false;
    }

    string password = getPasswordInput("Enter password: ");

    if (password.length() < 6)
    {
        cerr << "Password must be at least 6 characters!" << endl;
        return false;
    }

    string confirmPassword = getPasswordInput("Confirm password: ");

    if (password != confirmPassword)
    {
        cerr << "Passwords do not match!" << endl;
        return false;
    }

    return registerUser(username, password, role);
}

// ===== LOGIN =====
bool Auth::login(const string &username, const string &password)
{
    if (username.empty() || password.empty())
    {
        cerr << "Username and password cannot be empty!" << endl;
        return false;
    }

    if (!usernameExists(username))
    {
        cerr << "Username not found!" << endl;
        return false;
    }

    User &user = users[username];

    if (!verifyPassword(password, user.password))
    {
        cerr << "Wrong password!" << endl;
        return false;
    }

    currentUser = &user;

    cout << "\n\033[32m==============================================\033[0m" << endl;
    cout << "\033[32mLogin successful!\033[0m" << endl;
    cout << "Welcome, " << user.username << "!" << endl;
    cout << "Role: " << user.role << endl;
    cout << "\033[32m==============================================\033[0m\n"
         << endl;

    return true;
}

// ===== LOGIN WITH MASKED PASSWORD =====
bool Auth::loginWithMaskedPassword(const string &username)
{
    if (username.empty())
    {
        cerr << "Username cannot be empty!" << endl;
        return false;
    }

    if (!usernameExists(username))
    {
        cerr << "Username not found!" << endl;
        return false;
    }

    string password = getPasswordInput("Enter password: ");
    return login(username, password);
}

// ===== LOGOUT =====
void Auth::logout()
{
    if (currentUser == nullptr)
    {
        cout << "No user is currently logged in." << endl;
        return;
    }

    cout << "Goodbye, " << currentUser->username << "!" << endl;
    currentUser = nullptr;
}

// ===== IS LOGGED IN =====
bool Auth::isLoggedIn() const
{
    return currentUser != nullptr;
}

// ===== GET CURRENT USER =====
User *Auth::getCurrentUser()
{
    return currentUser;
}

// ===== IS ADMIN =====
bool Auth::isAdmin() const
{
    return currentUser != nullptr && currentUser->role == "admin";
}

// ===== IS USER =====
bool Auth::isUser() const
{
    return currentUser != nullptr && currentUser->role == "user";
}

// ===== GET ALL USERS =====
vector<User> Auth::getAllUsers() const
{
    vector<User> allUsers;
    for (const auto &pair : users)
    {
        allUsers.push_back(pair.second);
    }
    return allUsers;
}

// ===== DELETE USER =====
bool Auth::deleteUser(const string &username)
{
    if (!usernameExists(username))
    {
        cerr << "User '" << username << "' not found!" << endl;
        return false;
    }

    if (currentUser != nullptr && currentUser->username == username)
    {
        cerr << "Cannot delete yourself!" << endl;
        return false;
    }

    users.erase(username);
    cout << "User '" << username << "' deleted successfully!" << endl;
    return true;
}

// ===== UPDATE USER ROLE =====
bool Auth::updateUserRole(const string &username, const string &newRole)
{
    if (!usernameExists(username))
    {
        cerr << "User '" << username << "' not found!" << endl;
        return false;
    }

    if (newRole != "admin" && newRole != "user")
    {
        cerr << "Invalid role! Must be 'admin' or 'user'" << endl;
        return false;
    }

    if (currentUser != nullptr && currentUser->username == username)
    {
        cerr << "Cannot change your own role!" << endl;
        return false;
    }

    users[username].role = newRole;
    cout << "Role of '" << username << "' changed to '" << newRole << "'" << endl;
    return true;
}

// ===== LOAD DEFAULT USERS =====
void Auth::loadDefaultUsers()
{
    registerUser("admin", "admin123", "admin");

    cout << "Default users loaded:" << endl;
    cout << "- Admin: username=admin, password=admin123" << endl;
}

// ===== GET USER COUNT =====
int Auth::getUserCount() const
{
    return users.size();
}

// ===== DISPLAY CURRENT USER =====
void Auth::displayCurrentUser() const
{
    if (currentUser == nullptr)
    {
        cout << "No user logged in." << endl;
        return;
    }

    cout << "\n\033[36mCurrent User:\033[0m" << endl;
    cout << "Username: " << currentUser->username << endl;
    cout << "Role: " << currentUser->role << endl;
}

// ===== DISPLAY ALL USERS (FIXED!) =====
void Auth::displayAllUsers() const
{
    cout << "\n=== ALL REGISTERED USERS ===\n";
    cout << left << setw(5) << "ID"
         << setw(20) << "Username"
         << setw(10) << "Role" << endl;
    cout << string(35, '-') << endl;

    // FIXED: Loop dengan pair, bukan pointer!
    for (const auto &pair : users)
    {
        const User &user = pair.second;
        cout << left << setw(5) << user.id
             << setw(20) << user.username
             << setw(10) << user.role << endl;
    }

    cout << "\nTotal users: " << users.size() << endl;
}