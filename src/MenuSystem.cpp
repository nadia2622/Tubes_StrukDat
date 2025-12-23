#include "../include/MenuSystem.h"
#include "../include/SearchFilter.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

// ===== EXTERNAL FUNCTION DECLARATIONS =====
extern void clearScreen();
extern void pause();
extern void printHeader(const string &title);
extern void crudKomikMenu(BST &tree, KomikManager &manager);
extern void crudGenreMenu(KomikManager &manager, BST &tree);
extern void crudAuthorMenu(KomikManager &manager, BST &tree);
extern void traversalMenu(BST &tree);
extern void viewFavoritesMenu(KomikManager &manager, BST &tree);

// ===== CONSTRUCTOR =====
MenuSystem::MenuSystem(BST &tree, KomikManager &manager, Auth &auth)
    : tree(tree), manager(manager), auth(auth) {}

// ===== DESTRUCTOR =====
MenuSystem::~MenuSystem() {}

string selectMultipleGenres(KomikManager &manager)
{
    vector<string> allGenres = manager.getAllGenres();

    // Tampilkan daftar genre
    cout << "\n\033[1;36mAvailable genres:\033[0m\n";
    for (size_t i = 0; i < allGenres.size(); i++)
    {
        cout << (i + 1) << ". " << allGenres[i] << endl;
    }

    string input;
    vector<int> choices;

    cout << "\nSelect genres to filter (e.g., '1 3'): ";
    getline(cin, input);

    // Parsing input "1 3" jadi angka
    stringstream ss(input);
    int num;
    while (ss >> num)
    {
        if (num >= 1 && num <= (int)allGenres.size())
        {
            choices.push_back(num);
        }
    }

    if (choices.empty())
        return "";

    // Gabungkan jadi string "Action, Comedy"
    string result = "";
    for (size_t i = 0; i < choices.size(); i++)
    {
        result += allGenres[choices[i] - 1];
        if (i < choices.size() - 1)
            result += ", ";
    }

    cout << "\033[32mSelected: " << result << "\033[0m" << endl;
    return result;
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
    cin.ignore();

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

    do
    {
        printHeader("ADMIN MENU");

        auth.displayCurrentUser();
        cout << endl;

        cout << "Admin Panel - Full Access\n\n";
        cout << "1. CRUD Comic\n";
        cout << "2. CRUD Genre\n";
        cout << "3. CRUD Author\n";
        cout << "4. Tree Traversal\n";
        cout << "5. View All Favorites\n";
        cout << "6. User Management\n";
        cout << "7. System Statistics\n";
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
            traversalMenu(tree);
            break;
        case 5:
            viewFavoritesMenu(manager, tree);
            break;
        case 6:
            userManagementMenu();
            break;
        case 7:
            printHeader("SYSTEM STATISTICS");
            cout << "\033[1;36mStatistics:\033[0m\n\n";
            cout << "Total Comics: " << tree.count() << endl;
            cout << "Total Users: " << auth.getUserCount() << endl;
            cout << "Total Genres: " << manager.getAllGenres().size() << endl;
            cout << "Total Authors: " << manager.getAllAuthors().size() << endl;
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
    SearchFilter filter;

    do
    {
        printHeader("USER MENU");

        auth.displayCurrentUser();
        cout << endl;

        cout << "User Panel\n\n";
        cout << "1. Browse All Comics\n";
        cout << "2. Search & Filter Comics\n";
        cout << "3. My Favorites\n";
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
        {
            // ===== SEARCH & FILTER MENU =====
            int searchChoice;
            printHeader("SEARCH & FILTER");
            cout << "1. Search by Title\n";
            cout << "2. Search by Author\n";
            cout << "3. Filter by Genre (Multiple)\n"; // Update Text
            cout << "0. Back\n";
            cout << "\nChoice: ";
            cin >> searchChoice;
            cin.ignore();

            if (searchChoice == 0)
                break;

            vector<Komik *> results;
            string keyword;

            // Logic khusus untuk Genre (pakai menu pilihan angka)
            if (searchChoice == 3)
            {
                keyword = selectMultipleGenres(manager); // Panggil helper baru
                if (keyword.empty())
                {
                    cout << "\033[31mNo genre selected!\033[0m" << endl;
                    pause();
                    break;
                }
                results = filter.searchByGenre(tree, keyword);
                filter.displayResults(results, "Comics with genre(s): " + keyword);
            }
            // Logic untuk Title & Author (input manual)
            else
            {
                cout << "Enter keyword: ";
                getline(cin, keyword);

                if (searchChoice == 1)
                {
                    results = filter.searchByTitle(tree, keyword);
                    filter.displayResults(results, "Search Title: " + keyword);
                }
                else if (searchChoice == 2)
                {
                    results = filter.searchByAuthor(tree, keyword);
                    filter.displayResults(results, "Search Author: " + keyword);
                }
                else
                {
                    cout << "\033[31mInvalid option!\033[0m" << endl;
                }
            }

            pause();
            break;
        }
        case 3:
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
                cout << left << setw(5) << "ID"
                     << setw(20) << "Username"
                     << setw(10) << "Role" << endl;
                cout << string(35, '-') << endl;

                for (const auto &user : allUsers)
                {
                    cout << left << setw(5) << user.id
                         << setw(20) << user.username
                         << setw(10) << user.role << endl;
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
        cout << left << setw(5) << "ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(20) << "Genre(s)" << endl;
        cout << string(80, '-') << endl;

        tree.inOrder([](Komik *comic)
                     { cout << left << setw(5) << comic->id
                            << setw(30) << comic->title
                            << setw(25) << comic->author
                            << setw(20) << comic->genre << endl; });
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
            string keyword;
            cout << "Enter keyword to find comic: ";
            getline(cin, keyword);

            Komik *comic = tree.findByKeyword(keyword);
            if (comic != nullptr)
            {
                manager.addFavorite(currentUser->username, comic->id);
            }
            pause();
            break;
        }

        case 3:
        {
            printHeader("REMOVE FROM FAVORITES");
            string keyword;
            cout << "Enter keyword to find comic: ";
            getline(cin, keyword);

            Komik *comic = tree.findByKeyword(keyword);
            if (comic != nullptr)
            {
                manager.removeFavorite(currentUser->username, comic->id);
            }
            pause();
            break;
        }
        }
    } while (choice != 0);
}