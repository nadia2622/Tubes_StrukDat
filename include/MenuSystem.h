/*
 * MenuSystem.h
 * Menu System dengan Role-Based Access (Bagian Ryvanda)
 * 
 * Fitur:
 * 1. Menu berbeda untuk Admin & User
 * 2. Admin: Full access
 * 3. User: Limited access (view, search, favorites)
 */

#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "BST.h"
#include "KomikManager.h"
// #include "SearchFilter.h"k
#include "Auth.h"
#include <string>

using namespace std;

// ===== CLASS MenuSystem =====
// Class untuk mengelola menu berdasarkan role
class MenuSystem {
private:
    // ===== REFERENCES KE OBJEK UTAMA =====
    BST& tree;                    // Reference ke BST
    KomikManager& manager;        // Reference ke KomikManager
    // SearchFilter& searcher;       // Reference ke SearchFilter
    Auth& auth;                   // Reference ke Auth
    
public:
    // ===== CONSTRUCTOR =====
    // Constructor menerima reference ke semua objek yang dibutuhkan
    MenuSystem(BST& tree, KomikManager& manager, Auth& auth);
    
    // ===== DESTRUCTOR =====
    ~MenuSystem();
    
    // ===== MAIN MENU =====
    
    // showMainMenu: Tampilkan menu utama (sebelum login)
    // Menu: Login, Register, Exit
    void showMainMenu();
    
    // ===== ADMIN MENU =====
    
    // showAdminMenu: Tampilkan menu untuk admin
    // Menu: CRUD Comic, CRUD Genre, CRUD Author, User Management, dll
    void showAdminMenu();
    
    // ===== USER MENU =====
    
    // showUserMenu: Tampilkan menu untuk user biasa
    // Menu: View Comics, Search, Filter, My Favorites, dll
    void showUserMenu();
    
    // ===== USER-SPECIFIC FEATURES =====
    
    // myFavoritesMenu: Menu untuk manage favorites user
    // User bisa add/remove favorites
    void myFavoritesMenu();
    
    // viewComicsMenu: Menu untuk view & browse komik (read-only untuk user)
    void viewComicsMenu();
    
    // ===== ADMIN-SPECIFIC FEATURES =====
    
    // userManagementMenu: Menu untuk manage users (admin only)
    // Admin bisa view all users, delete user, change role
    void userManagementMenu();
    
    // ===== AUTHENTICATION FLOW =====
    
    // handleLogin: Handle login process
    // Return: true jika login berhasil
    bool handleLogin();
    
    // handleRegister: Handle register process
    // Return: true jika register berhasil
    bool handleRegister();
};

#endif // MENU_SYSTEM_H

/*
 * ===== PENJELASAN KONSEP =====
 * 
 * 1. ROLE-BASED MENU:
 *    - Admin: Full access ke semua fitur
 *    - User: Limited access (read-only untuk komik, manage favorites)
 * 
 * 2. MENU HIERARCHY:
 *    Main Menu (sebelum login)
 *    ├── Login → Admin Menu / User Menu
 *    ├── Register → kembali ke Main Menu
 *    └── Exit
 * 
 *    Admin Menu:
 *    ├── CRUD Comic (full access)
 *    ├── CRUD Genre (full access)
 *    ├── CRUD Author (full access)
 *    ├── User Management (view, delete, change role)
 *    ├── Search & Filter
 *    ├── View All Favorites
 *    └── Logout
 * 
 *    User Menu:
 *    ├── View All Comics (read-only)
 *    ├── Search Comic
 *    ├── Filter Comics
 *    ├── My Favorites (add/remove)
 *    └── Logout
 * 
 * 3. REFERENCE MEMBERS:
 *    - Pakai reference (&) bukan pointer (*)
 *    - Kenapa? Karena objek pasti ada (tidak bisa null)
 *    - Reference tidak perlu -> untuk akses member
 * 
 * 4. SEPARATION OF CONCERNS:
 *    - Auth: Handle authentication & session
 *    - MenuSystem: Handle menu flow & user interaction
 *    - BST, Manager, Searcher: Handle business logic
 */