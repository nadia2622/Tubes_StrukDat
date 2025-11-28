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

// ===== STRUCT USER =====
// Struktur untuk menyimpan data user
struct User
{
    int id;          // ID user (unique)
    string username; // Username (unique, untuk login)
    string password; // Password (plain text untuk simplicity)
    string role;     // Role: "admin" atau "user"

    // Constructor
    User() : id(0), username(""), password(""), role("user") {}

    User(int id, string username, string password, string role)
        : id(id), username(username), password(password), role(role) {}

    // Method untuk display user info (tanpa password)
    void display() const
    {
        cout << "ID: " << id
             << " | Username: " << username
             << " | Role: " << role << endl;
    }
};

// ===== CLASS AUTH =====
// Class untuk authentication & user management
class Auth
{
private:
    // ===== PRIVATE MEMBERS =====

    // Map untuk simpan semua user
    // Key: username (unique)
    // Value: User object
    map<string, User> users;

    // ID counter untuk auto-increment
    int nextUserId;

    // Current logged in user (nullptr jika belum login)
    User *currentUser;

    // ===== PRIVATE HELPER METHODS =====

    // Hash password (untuk security)
    // Catatan: Ini implementasi sederhana, production pakai bcrypt/argon2
    string hashPassword(const string &password);

    // Verify password
    bool verifyPassword(const string &inputPassword, const string &storedPassword);

    // Check if username already exists
    bool usernameExists(const string &username);

    // Function untuk input password dengan masking (tampilkan *)
    string getPasswordInput(const string &prompt);

public:
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    Auth();
    ~Auth();

    // ===== AUTHENTICATION METHODS =====

    // register: Daftarkan user baru
    // Parameter:
    //   - username: username (harus unique)
    //   - password: password (akan di-hash)
    //   - role: "admin" atau "user" (default: "user")
    // Return: true jika berhasil, false jika gagal (username sudah ada)
    bool registerUser(const string &username, const string &password, const string &role = "user");

    // login: Login user
    // Parameter:
    //   - username: username
    //   - password: password
    // Return: true jika berhasil, false jika gagal (username/password salah)
    bool login(const string &username, const string &password);

    // Login dengan input password yang di-mask (tampil ***)
    bool loginWithMaskedPassword(const string &username);

    // Register dengan input password yang di-mask
    bool registerWithMaskedPassword(const string &username, const string &role = "user");

    // logout: Logout user saat ini
    void logout();

    // isLoggedIn: Cek apakah ada user yang login
    // Return: true jika ada user login, false jika belum
    bool isLoggedIn() const;

    // ===== SESSION MANAGEMENT =====

    // getCurrentUser: Ambil user yang sedang login
    // Return: pointer ke User object (nullptr jika belum login)
    User *getCurrentUser();

    // isAdmin: Cek apakah user saat ini admin
    // Return: true jika admin, false jika bukan atau belum login
    bool isAdmin() const;

    // isUser: Cek apakah user saat ini user biasa
    // Return: true jika user biasa, false jika admin atau belum login
    bool isUser() const;

    // ===== USER MANAGEMENT (ADMIN ONLY) =====

    // getAllUsers: Ambil semua user (untuk admin)
    // Return: vector berisi semua user
    vector<User> getAllUsers() const;

    // deleteUser: Hapus user (admin only)
    // Parameter: username yang mau dihapus
    // Return: true jika berhasil, false jika gagal
    bool deleteUser(const string &username);

    // updateUserRole: Ubah role user (admin only)
    // Parameter:
    //   - username: username yang mau diubah
    //   - newRole: role baru ("admin" atau "user")
    // Return: true jika berhasil, false jika gagal
    bool updateUserRole(const string &username, const string &newRole);

    // ===== UTILITY =====

    // loadDefaultUsers: Load default users (admin & sample users)
    // Dipanggil saat inisialisasi
    void loadDefaultUsers();

    // getUserCount: Hitung total user
    // Return: jumlah user
    int getUserCount() const;

    // displayCurrentUser: Tampilkan info user yang login
    void displayCurrentUser() const;
};

#endif // AUTH_H

/*
 * ===== PENJELASAN KONSEP =====
 *
 * 1. AUTHENTICATION:
 *    - Login: Verifikasi username & password
 *    - Register: Buat akun baru
 *    - Logout: Keluar dari akun
 *
 * 2. SESSION MANAGEMENT:
 *    - Simpan info user yang sedang login
 *    - Cek role (admin atau user)
 *    - Logout akan clear session
 *
 * 3. ROLE-BASED ACCESS:
 *    - Admin: Full access (CRUD semua, user management)
 *    - User: Limited access (view, search, favorites)
 *
 * 4. PASSWORD SECURITY:
 *    - Jangan simpan plain text (ini untuk demo)
 *    - Production pakai bcrypt/argon2 untuk hash
 *    - Pakai salt untuk security tambahan
 *
 * 5. MAP untuk USER STORAGE:
 *    - Key: username (unique)
 *    - Value: User object
 *    - Fast lookup: O(log n)
 *    - Easy check username exists
 */