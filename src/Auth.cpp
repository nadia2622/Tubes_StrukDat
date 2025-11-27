/*
 * Auth.cpp
 * Implementasi Authentication System
 */

#include "../include/Auth.h"
#include <iostream>
#include <algorithm>  // Untuk transform()

using namespace std;

// ===== CONSTRUCTOR =====
Auth::Auth() {
    // Inisialisasi
    nextUserId = 1;          // ID dimulai dari 1
    currentUser = nullptr;   // Belum ada user yang login
    
    // Load default users (admin & sample users)
    loadDefaultUsers();
}

// ===== DESTRUCTOR =====
Auth::~Auth() {
    // Tidak ada cleanup khusus
    // Map otomatis dibersihkan oleh C++
}

// ===== HASH PASSWORD =====
// Fungsi untuk "hash" password (implementasi sederhana)
// CATATAN: Ini untuk demo! Production pakai bcrypt/argon2!
string Auth::hashPassword(const string& password) {
    // Implementasi sederhana: reverse string + tambah prefix
    // Ini TIDAK aman! Hanya untuk demo!
    
    string hashed = password;
    
    // Reverse string
    // reverse() = fungsi dari <algorithm> untuk balik urutan
    reverse(hashed.begin(), hashed.end());
    
    // Tambah prefix "HASH_"
    hashed = "HASH_" + hashed;
    
    return hashed;
    
    // Contoh: "admin123" → "HASH_321nimda"
}

// ===== VERIFY PASSWORD =====
// Fungsi untuk verifikasi password
bool Auth::verifyPassword(const string& inputPassword, const string& storedPassword) {
    // Hash input password
    string hashedInput = hashPassword(inputPassword);
    
    // Bandingkan dengan stored password
    return hashedInput == storedPassword;
}

// ===== CHECK USERNAME EXISTS =====
// Cek apakah username sudah ada
bool Auth::usernameExists(const string& username) {
    // find() = cari key di map
    // Return: iterator jika ketemu, end() jika tidak
    return users.find(username) != users.end();
}

// ===== REGISTER USER =====
// Fungsi untuk register user baru
bool Auth::registerUser(const string& username, const string& password, const string& role) {
    // ===== VALIDASI INPUT =====
    
    // 1. Cek username tidak kosong
    if (username.empty()) {
        cerr << "Username cannot be empty!" << endl;
        return false;
    }
    
    // 2. Cek password tidak kosong
    if (password.empty()) {
        cerr << "Password cannot be empty!" << endl;
        return false;
    }
    
    // 3. Cek username minimal 3 karakter
    if (username.length() < 3) {
        cerr << "Username must be at least 3 characters!" << endl;
        return false;
    }
    
    // 4. Cek password minimal 6 karakter
    if (password.length() < 6) {
        cerr << "Password must be at least 6 characters!" << endl;
        return false;
    }
    
    // 5. Cek username belum ada
    if (usernameExists(username)) {
        cerr << "Username '" << username << "' already exists!" << endl;
        return false;
    }
    
    // 6. Cek role valid
    if (role != "admin" && role != "user") {
        cerr << "Invalid role! Must be 'admin' or 'user'" << endl;
        return false;
    }
    
    // ===== BUAT USER BARU =====
    
    // Hash password
    string hashedPassword = hashPassword(password);
    
    // Bikin User object baru
    User newUser(nextUserId++, username, hashedPassword, role);
    
    // Simpan ke map
    // users[username] = newUser akan:
    // 1. Bikin entry baru jika username belum ada
    // 2. Assign newUser ke entry tersebut
    users[username] = newUser;
    
    cout << "User '" << username << "' registered successfully!" << endl;
    return true;
}

// ===== LOGIN =====
// Fungsi untuk login user
bool Auth::login(const string& username, const string& password) {
    // ===== VALIDASI INPUT =====
    
    if (username.empty() || password.empty()) {
        cerr << "Username and password cannot be empty!" << endl;
        return false;
    }
    
    // ===== CEK USERNAME ADA =====
    if (!usernameExists(username)) {
        cerr << "Username not found!" << endl;
        return false;
    }
    
    // ===== AMBIL USER DARI MAP =====
    // users[username] = akses User object dengan key username
    User& user = users[username];
    // & = reference (bukan copy, langsung akses yang di map)
    
    // ===== VERIFY PASSWORD =====
    if (!verifyPassword(password, user.password)) {
        cerr << "Wrong password!" << endl;
        return false;
    }
    
    // ===== LOGIN BERHASIL =====
    // Set current user
    currentUser = &user;
    // &user = ambil alamat/pointer dari user
    
    cout << "\n\033[32m==============================================\033[0m" << endl;
    cout << "\033[32mLogin successful!\033[0m" << endl;
    cout << "Welcome, " << user.username << "!" << endl;
    cout << "Role: " << user.role << endl;
    cout << "\033[32m==============================================\033[0m\n" << endl;
    
    return true;
}

// ===== LOGOUT =====
// Fungsi untuk logout
void Auth::logout() {
    if (currentUser == nullptr) {
        cout << "No user is currently logged in." << endl;
        return;
    }
    
    cout << "Goodbye, " << currentUser->username << "!" << endl;
    currentUser = nullptr;  // Clear session
}

// ===== IS LOGGED IN =====
// Cek apakah ada user yang login
bool Auth::isLoggedIn() const {
    // Return true jika currentUser tidak null
    return currentUser != nullptr;
}

// ===== GET CURRENT USER =====
// Ambil user yang sedang login
User* Auth::getCurrentUser() {
    return currentUser;
}

// ===== IS ADMIN =====
// Cek apakah user saat ini admin
bool Auth::isAdmin() const {
    // Cek 2 kondisi:
    // 1. Ada user yang login (currentUser != nullptr)
    // 2. Role user adalah "admin"
    return currentUser != nullptr && currentUser->role == "admin";
}

// ===== IS USER =====
// Cek apakah user saat ini user biasa
bool Auth::isUser() const {
    return currentUser != nullptr && currentUser->role == "user";
}

// ===== GET ALL USERS =====
// Ambil semua user (untuk admin)
vector<User> Auth::getAllUsers() const {
    // Bikin vector kosong
    vector<User> allUsers;
    
    // Loop semua entry di map
    // for (const auto& pair : users)
    // pair = pasangan key-value
    // pair.first = key (username)
    // pair.second = value (User object)
    for (const auto& pair : users) {
        allUsers.push_back(pair.second);  // Tambah User ke vector
    }
    
    return allUsers;
}

// ===== DELETE USER =====
// Hapus user (admin only)
bool Auth::deleteUser(const string& username) {
    // ===== VALIDASI =====
    
    // 1. Cek username ada
    if (!usernameExists(username)) {
        cerr << "User '" << username << "' not found!" << endl;
        return false;
    }
    
    // 2. Tidak boleh hapus diri sendiri
    if (currentUser != nullptr && currentUser->username == username) {
        cerr << "Cannot delete yourself!" << endl;
        return false;
    }
    
    // ===== HAPUS USER =====
    // erase() = hapus entry dengan key tertentu dari map
    users.erase(username);
    
    cout << "User '" << username << "' deleted successfully!" << endl;
    return true;
}

// ===== UPDATE USER ROLE =====
// Ubah role user (admin only)
bool Auth::updateUserRole(const string& username, const string& newRole) {
    // ===== VALIDASI =====
    
    // 1. Cek username ada
    if (!usernameExists(username)) {
        cerr << "User '" << username << "' not found!" << endl;
        return false;
    }
    
    // 2. Cek role valid
    if (newRole != "admin" && newRole != "user") {
        cerr << "Invalid role! Must be 'admin' or 'user'" << endl;
        return false;
    }
    
    // 3. Tidak boleh ubah role diri sendiri
    if (currentUser != nullptr && currentUser->username == username) {
        cerr << "Cannot change your own role!" << endl;
        return false;
    }
    
    // ===== UPDATE ROLE =====
    users[username].role = newRole;
    
    cout << "Role of '" << username << "' changed to '" << newRole << "'" << endl;
    return true;
}

// ===== LOAD DEFAULT USERS =====
// Load default users saat inisialisasi
void Auth::loadDefaultUsers() {
    // ===== ADMIN DEFAULT =====
    // Username: admin
    // Password: admin123
    registerUser("admin", "admin123", "admin");
    
    // ===== SAMPLE USERS =====
    // Username: user1, Password: user123
    registerUser("user1", "user123", "user");
    
    // Username: user2, Password: user123
    registerUser("user2", "user123", "user");
    
    cout << "Default users loaded:" << endl;
    cout << "- Admin: username=admin, password=admin123" << endl;
    cout << "- User1: username=user1, password=user123" << endl;
    cout << "- User2: username=user2, password=user123" << endl;
}

// ===== GET USER COUNT =====
// Hitung total user
int Auth::getUserCount() const {
    // size() = ukuran map (jumlah entry)
    return users.size();
}

// ===== DISPLAY CURRENT USER =====
// Tampilkan info user yang login
void Auth::displayCurrentUser() const {
    if (currentUser == nullptr) {
        cout << "No user logged in." << endl;
        return;
    }
    
    cout << "\n\033[36mCurrent User:\033[0m" << endl;
    cout << "Username: " << currentUser->username << endl;
    cout << "Role: " << currentUser->role << endl;
}

/*
 * ===== RINGKASAN Auth.cpp =====
 * 
 * 1. HASH PASSWORD:
 *    - Implementasi sederhana (reverse + prefix)
 *    - Production pakai bcrypt/argon2!
 *    - Contoh: "admin123" → "HASH_321nimda"
 * 
 * 2. REGISTER:
 *    - Validasi input (username min 3 char, password min 6 char)
 *    - Cek username belum ada
 *    - Hash password
 *    - Simpan ke map
 * 
 * 3. LOGIN:
 *    - Cek username ada
 *    - Verify password (hash input lalu compare)
 *    - Set currentUser jika berhasil
 * 
 * 4. SESSION:
 *    - currentUser = pointer ke User yang login
 *    - nullptr jika belum login
 *    - Logout akan set currentUser = nullptr
 * 
 * 5. ROLE CHECK:
 *    - isAdmin(): cek currentUser != nullptr && role == "admin"
 *    - isUser(): cek currentUser != nullptr && role == "user"
 * 
 * 6. USER MANAGEMENT:
 *    - getAllUsers(): loop map, push ke vector
 *    - deleteUser(): erase dari map
 *    - updateUserRole(): ubah field role
 * 
 * 7. DEFAULT USERS:
 *    - admin/admin123 (admin)
 *    - user1/user123 (user)
 *    - user2/user123 (user)
 * 
 * ===== KONSEP PENTING =====
 * 
 * MAP OPERATIONS:
 *    map<string, User> users;
 *    
 *    // Insert/Update
 *    users[username] = newUser;
 *    
 *    // Access
 *    User& user = users[username];
 *    
 *    // Check exists
 *    users.find(username) != users.end()
 *    
 *    // Delete
 *    users.erase(username);
 *    
 *    // Loop
 *    for (const auto& pair : users) {
 *        pair.first;   // key (username)
 *        pair.second;  // value (User)
 *    }
 * 
 * POINTER vs REFERENCE:
 *    User* ptr = &user;     // Pointer (bisa null, bisa ganti alamat)
 *    User& ref = user;      // Reference (alias, tidak bisa null)
 *    
 *    ptr->username;         // Akses member via pointer
 *    ref.username;          // Akses member via reference
 */