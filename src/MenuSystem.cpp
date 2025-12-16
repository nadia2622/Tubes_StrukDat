/*
 * MenuSystem.cpp - COMPLETE VERSION (FIXED)
 * * PENTING: File ini HANYA include header (.h), TIDAK include .cpp!
 */

#include "../include/MenuSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>

// Pastikan SearchFilter.h ter-include secara tidak langsung via MenuSystem.h
// atau kita asumsikan class SearchFilter sudah dikenali.
// Jika masih error tipe data, pastikan include header SearchFilter ada di MenuSystem.h

using namespace std;

// ===== EXTERNAL FUNCTION DECLARATIONS =====
// Fungsi-fungsi ini ada di main.cpp
extern void clearScreen();
extern void pause();
extern void printHeader(const string &title);
extern void crudKomikMenu(BST &tree, KomikManager &manager);
extern void crudGenreMenu(KomikManager &manager, BST &tree);
extern void crudAuthorMenu(KomikManager &manager, BST &tree);
extern void traversalMenu(BST &tree);
extern void viewFavoritesMenu(KomikManager &manager, BST &tree);
extern void searchFilterMenu(BST &tree, SearchFilter &searcher);

// ===== CONSTRUCTOR =====
MenuSystem::MenuSystem(BST &tree, KomikManager &manager, Auth &auth)
    : tree(tree), manager(manager), auth(auth)
{
}

// ===== DESTRUCTOR =====
MenuSystem::~MenuSystem()
{
}

// ===== MAIN MENU =====
void MenuSystem::showMainMenu()
{
    int choice;

    do
    {
        printHeader("KOMIK MANAGEMENT SYSTEM");

        cout << "Welcome! Please choose an option:\n\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. About System\n";
        cout << "0. Exit\n";
        cout << "\nChoice: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
        {
            if (handleLogin())
            {
                if (auth.isAdmin())
                {
                    showAdminMenu();
                }
                else
                {
                    showUserMenu();
                }
            }
            else
            {
                cout << "\033[31mLogin failed!\033[0m" << endl;
                pause();
            }
            break;
        }

        case 2:
        {
            handleRegister();
            break;
        }

        case 3:
        {
            printHeader("ABOUT SYSTEM");

            cout << "\033[1;36mKomik Management System\033[0m\n\n";
            cout << "Version: 1.0\n";
            cout << "Developed by: Tim Tubes Struktur Data\n\n";

            cout << "Features:\n";
            cout << "- Binary Search Tree implementation\n";
            cout << "- CRUD operations\n";
            cout << "- Search & Filter (DFS)\n";
            cout << "- Authentication & Authorization\n";
            cout << "- Role-based access control\n\n";

            cout << "Default Accounts:\n";
            cout << "- Admin: username=admin, password=admin123\n";
            cout << "- User1: username=user1, password=user123\n";

            pause();
            break;
        }

        case 0:
            cout << "\n\033[36mThank you!\033[0m" << endl;
            break;

        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

// ===== HANDLE LOGIN =====
bool MenuSystem::handleLogin()
{
    printHeader("LOGIN");

    string username;

    cout << "Username: ";
    cin >> username;
    cin.ignore();

    bool success = auth.loginWithMaskedPassword(username);

    if (success)
    {
        pause();
        return true;
    }
    else
    {
        pause();
        return false;
    }
}

// ===== HANDLE REGISTER =====
bool MenuSystem::handleRegister()
{
    printHeader("REGISTER NEW ACCOUNT");

    string username;

    cout << "Enter username (min 3 characters): ";
    cin >> username;
    cin.ignore(); // Clear newline buffer

    bool success = auth.registerWithMaskedPassword(username, "user");

    if (success)
    {
        cout << "\n\033[32mRegistration successful!\033[0m" << endl;
        cout << "You can now login with your account." << endl;
        pause();
        return true;
    }
    else
    {
        pause();
        return false;
    }
}

// ===== ADMIN MENU =====
void MenuSystem::showAdminMenu()
{
    int choice;
    SearchFilter searcher; // FIX: Instansiasi objek searcher lokal untuk digunakan di menu ini

    do
    {
        printHeader("ADMIN MENU");

        auth.displayCurrentUser();
        cout << endl;

        cout << "Admin Panel - Full Access\n\n";
        cout << "1. CRUD Comic\n";
        cout << "2. CRUD Genre\n";
        cout << "3. CRUD Author\n";
        cout << "4. Search & Filter Comics\n";
        cout << "5. Tree Traversal\n";
        cout << "6. User Management\n";
        cout << "7. View All Favorites\n";
        cout << "8. Reload Sample Data\n";
        cout << "9. System Statistics\n";
        cout << "0. Logout\n";
        cout << "\nChoice: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            crudKomikMenu(tree, manager);
            break;
        case 2:
            crudGenreMenu(manager, tree);
            break;
        case 3:
            crudAuthorMenu(manager, tree);
            break;
        case 4:
            searchFilterMenu(tree, searcher); // searcher sekarang sudah dideklarasikan di atas
            break;
        case 5:
            traversalMenu(tree);
            break;
        case 6:
            userManagementMenu();
            break;
        case 7:
            viewFavoritesMenu(manager, tree);
            break;
        case 8:
            printHeader("RELOAD SAMPLE DATA");
            manager.loadSampleData(tree);
            cout << "\033[32mSample data reloaded!\033[0m" << endl;
            pause();
            break;
        case 9:
            printHeader("SYSTEM STATISTICS");
            cout << "\033[1;36mStatistics:\033[0m\n\n";
            cout << "Total Comics: " << tree.count() << endl;
            cout << "Total Users: " << auth.getUserCount() << endl;
            pause();
            break;
        case 0:
            auth.logout();
            cout << "\n\033[32mLogged out successfully!\033[0m" << endl;
            pause();
            break;
        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

// ===== USER MENU =====
void MenuSystem::showUserMenu()
{
    int choice;
    SearchFilter searcher; // FIX: Instansiasi objek searcher lokal

    do
    {
        printHeader("USER MENU");

        auth.displayCurrentUser();
        cout << endl;

        cout << "User Panel\n\n";
        cout << "1. Browse All Comics\n";
        cout << "2. Search Comic\n";
        cout << "3. Filter Comics\n";
        cout << "4. My Favorites\n";
        cout << "0. Logout\n";
        cout << "\nChoice: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            viewComicsMenu();
            break;
        case 2:
            searchFilterMenu(tree, searcher); // searcher aman digunakan
            break;
        case 3:
            searchFilterMenu(tree, searcher); // searcher aman digunakan
            break;
        case 4:
            myFavoritesMenu();
            break;
        case 0:
            auth.logout();
            cout << "\n\033[32mLogged out successfully!\033[0m" << endl;
            pause();
            break;
        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

// ===== USER MANAGEMENT MENU =====
void MenuSystem::userManagementMenu()
{
    int choice;

    do
    {
        printHeader("USER MANAGEMENT");

        cout << "1. View All Users\n";
        cout << "2. Delete User\n";
        cout << "3. Change User Role\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("ALL USERS");
            vector<User> allUsers = auth.getAllUsers();

            if (allUsers.empty())
            {
                cout << "\033[33mNo users found!\033[0m" << endl;
            }
            else
            {
                cout << left << setw(5) << "ID" << setw(20) << "Username" << setw(10) << "Role" << endl;
                cout << string(35, '-') << endl;

                for (const auto &user : allUsers)
                {
                    cout << left << setw(5) << user.id << setw(20) << user.username << setw(10) << user.role << endl;
                }
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("DELETE USER");
            string username;
            cout << "Enter username to delete: ";
            getline(cin, username);

            auth.deleteUser(username);
            pause();
            break;
        }

        case 3:
        {
            printHeader("CHANGE USER ROLE");
            string username, newRole;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter new role (admin/user): ";
            getline(cin, newRole);

            auth.updateUserRole(username, newRole);
            pause();
            break;
        }
        }
    } while (choice != 0);
}

// ===== VIEW COMICS MENU =====
void MenuSystem::viewComicsMenu()
{
    printHeader("BROWSE ALL COMICS");

    if (tree.isEmpty())
    {
        cout << "\033[33mNo comics available!\033[0m" << endl;
    }
    else
    {
        cout << left << setw(5) << "ID" << setw(30) << "Title" << setw(25) << "Author" << setw(15) << "Genre" << endl;
        cout << string(75, '-') << endl;

        tree.inOrder([](Komik *comic)
                     { cout << left << setw(5) << comic->id << setw(30) << comic->title << setw(25) << comic->author << setw(15) << comic->genre << endl; });
    }

    pause();
}

// ===== MY FAVORITES MENU =====
void MenuSystem::myFavoritesMenu()
{
    int choice;

    do
    {
        printHeader("MY FAVORITES");

        User *currentUser = auth.getCurrentUser();

        cout << "1. View My Favorites\n";
        cout << "2. Add to Favorites\n";
        cout << "3. Remove from Favorites\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("MY FAVORITES");
            vector<int> favorites = manager.getFavoritesByUser(currentUser->username);

            if (favorites.empty())
            {
                cout << "\033[33mYou don't have any favorites yet!\033[0m" << endl;
            }
            else
            {
                for (int id : favorites)
                {
                    tree.inOrder([id](Komik *c)
                                 {
                            if (c->id == id) c->display(); });
                }
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("ADD TO FAVORITES");
            string title;
            cout << "Enter comic title: ";
            getline(cin, title);

            Komik *comic = tree.search(title);
            if (comic != nullptr)
            {
                manager.addFavorite(currentUser->username, comic->id);
            }
            else
            {
                cout << "\033[31mComic not found!\033[0m" << endl;
            }
            pause();
            break;
        }

        case 3:
        {
            printHeader("REMOVE FROM FAVORITES");
            string title;
            cout << "Enter comic title: ";
            getline(cin, title);

            Komik *comic = tree.search(title);
            if (comic != nullptr)
            {
                manager.removeFavorite(currentUser->username, comic->id);
            }
            else
            {
                cout << "\033[31mComic not found!\033[0m" << endl;
            }
            pause();
            break;
        }
        }
    } while (choice != 0);
}