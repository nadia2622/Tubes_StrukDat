/*
 * Auth.h
 * File DEKLARASI untuk Class Auth
 *
 * Class ini menangani logika autentikasi:
 * - Validasi kredensial
 * - Registrasi user baru
 * - Menyimpan list users
 */

#ifndef AUTH_H
#define AUTH_H

#include "Login.h" // Include Login.h untuk akses struct User
#include <vector>
#include <string>

using namespace std;

// ===== CLASS AUTH =====
// Class untuk mengelola autentikasi dan user management
class Auth
{
private:
    // ===== PRIVATE MEMBERS =====
    vector<User *> users; // Vector untuk simpan semua users
    int nextUserId;       // ID untuk user baru

    User *currentUser; // Pointer ke user yang sedang login

public:
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    Auth();
    ~Auth();

    // ===== AUTHENTICATION METHODS =====

    // Autentikasi user dengan username dan password
    // Return: pointer ke User jika valid, nullptr jika tidak valid
    User *authenticate(const string &username, const string &password);

    // Registrasi user baru (role: user)
    // Return: true jika berhasil, false jika username sudah ada
    bool registerUser(const string &username, const string &password);

    // Tambah admin baru (dipanggil dari kode, bukan dari menu)
    // Return: true jika berhasil, false jika username sudah ada
    bool addAdmin(const string &username, const string &password);

    // ===== USER MANAGEMENT =====

    // Cek apakah username sudah ada
    bool usernameExists(const string &username) const;

    // Get user by username
    User *getUserByUsername(const string &username) const;

    // Get current logged in user
    User *getCurrentUser() const;

    // Set current user (setelah login berhasil)
    void setCurrentUser(User *user);

    // Logout (set currentUser ke nullptr)
    void logout();

    // ===== UTILITY =====

    // Get role dari current user
    string getCurrentUserRole() const;

    // Cek apakah current user adalah admin
    bool isAdmin() const;

    // Display semua users
    void displayAllUsers() const;

    // Get total users
    int getTotalUsers() const;
};

#endif // AUTH_H
