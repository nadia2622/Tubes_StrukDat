/*
 * Auth.cpp
 * File IMPLEMENTASI untuk Class Auth
 *
 * Class ini menangani logika autentikasi:
 * - Validasi kredensial
 * - Registrasi user baru
 * - Menyimpan list users
 */

#include "../include/Auth.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ===== CONSTRUCTOR =====
// Inisialisasi dengan 1 admin default
Auth::Auth()
{
    nextUserId = 1;    // ID pertama adalah 1
    currentUser = nullptr; // Belum ada yang login

    // ===== TAMBAH ADMIN DEFAULT =====
    // Admin default: username "admin", password "admin123"
    User *defaultAdmin = new User(nextUserId++, "admin", "admin123", "admin");
    users.push_back(defaultAdmin);

    cout << "\033[32m[Auth] System initialized with default admin account.\033[0m\n";
    cout << "\033[33m       Username: admin | Password: admin123\033[0m\n";
}

// ===== DESTRUCTOR =====
// Hapus semua user dari memory
Auth::~Auth()
{
    for (User *user : users)
    {
        delete user;
    }
    users.clear();
}

// ===== AUTHENTICATE =====
// Validasi username dan password
// Return: pointer ke User jika valid, nullptr jika tidak valid
User *Auth::authenticate(const string &username, const string &password)
{
    // Loop semua users
    for (User *user : users)
    {
        // Cek apakah username dan password cocok
        if (user->username == username && user->password == password)
        {
            return user; // Return pointer ke user yang valid
        }
    }

    return nullptr; // Tidak ditemukan
}

// ===== REGISTER USER =====
// Registrasi user baru dengan role "user"
// Return: true jika berhasil, false jika username sudah ada
bool Auth::registerUser(const string &username, const string &password)
{
    // Cek apakah username sudah ada
    if (usernameExists(username))
    {
        cout << "\033[31mError: Username '" << username << "' already exists!\033[0m\n";
        return false;
    }

    // Validasi input tidak kosong
    if (username.empty() || password.empty())
    {
        cout << "\033[31mError: Username and password cannot be empty!\033[0m\n";
        return false;
    }

    // Buat user baru dengan role "user"
    User *newUser = new User(nextUserId++, username, password, "user");
    users.push_back(newUser);

    cout << "\033[32mSuccess: Account '" << username << "' registered successfully!\033[0m\n";
    return true;
}

// ===== ADD ADMIN =====
// Tambah admin baru (dipanggil dari kode)
// Return: true jika berhasil, false jika username sudah ada
bool Auth::addAdmin(const string &username, const string &password)
{
    // Cek apakah username sudah ada
    if (usernameExists(username))
    {
        cout << "\033[31mError: Username '" << username << "' already exists!\033[0m\n";
        return false;
    }

    // Validasi input tidak kosong
    if (username.empty() || password.empty())
    {
        cout << "\033[31mError: Username and password cannot be empty!\033[0m\n";
        return false;
    }

    // Buat user baru dengan role "admin"
    User *newAdmin = new User(nextUserId++, username, password, "admin");
    users.push_back(newAdmin);

    cout << "\033[32mSuccess: Admin '" << username << "' added successfully!\033[0m\n";
    return true;
}

// ===== USERNAME EXISTS =====
// Cek apakah username sudah ada
bool Auth::usernameExists(const string &username) const
{
    for (const User *user : users)
    {
        if (user->username == username)
        {
            return true;
        }
    }
    return false;
}

// ===== GET USER BY USERNAME =====
User *Auth::getUserByUsername(const string &username) const
{
    for (User *user : users)
    {
        if (user->username == username)
        {
            return user;
        }
    }
    return nullptr;
}

// ===== GET CURRENT USER =====
User *Auth::getCurrentUser() const
{
    return currentUser;
}

// ===== SET CURRENT USER =====
void Auth::setCurrentUser(User *user)
{
    currentUser = user;
}

// ===== LOGOUT =====
void Auth::logout()
{
    currentUser = nullptr;
}

// ===== GET CURRENT USER ROLE =====
string Auth::getCurrentUserRole() const
{
    if (currentUser != nullptr)
    {
        return currentUser->role;
    }
    return "";
}

// ===== IS ADMIN =====
bool Auth::isAdmin() const
{
    if (currentUser != nullptr)
    {
        return currentUser->role == "admin";
    }
    return false;
}

// ===== DISPLAY ALL USERS =====
void Auth::displayAllUsers() const
{
    cout << "\n=== ALL REGISTERED USERS ===\n";
    cout << left << setw(5) << "ID"
         << setw(20) << "Username"
         << setw(10) << "Role" << endl;
    cout << string(35, '-') << endl;

    for (const User *user : users)
    {
        cout << left << setw(5) << user->id
             << setw(20) << user->username
             << setw(10) << user->role << endl;
    }

    cout << "\nTotal users: " << users.size() << endl;
}

// ===== GET TOTAL USERS =====
int Auth::getTotalUsers() const
{
    return users.size();
}
