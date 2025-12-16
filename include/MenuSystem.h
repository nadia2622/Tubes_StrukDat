#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "BST.h"
#include "KomikManager.h"
#include "Auth.h"
#include <string>

using namespace std;

class MenuSystem
{
private:
    BST &tree;
    KomikManager &manager;
    Auth &auth;

public:
    MenuSystem(BST &tree, KomikManager &manager, Auth &auth);
    ~MenuSystem();

    void showMainMenu();
    void showAdminMenu();
    void showUserMenu();

    void myFavoritesMenu();
    void viewComicsMenu();
    void userManagementMenu();

    bool handleLogin();
    bool handleRegister();
};

#endif