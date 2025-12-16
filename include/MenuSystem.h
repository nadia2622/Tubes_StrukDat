#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "BST.h"
#include "KomikManager.h"
// #include "SearchFilter.h"k
#include "Auth.h"
#include <string>

using namespace std;
class MenuSystem {
private:
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