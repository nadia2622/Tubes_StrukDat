/*
 * main.cpp - COMPLETE VERSION WITH SQLITE DATABASE
 * Komik Management System with Authentication & Database Integration
 */

// ===== INCLUDE HEADER FILES =====
#include "include/Komik.h"
#include "include/BST.h"
#include "include/KomikManager.h"
#include "include/Auth.h"
#include "include/MenuSystem.h"
#include "include/SearchFilter.h"
#include "include/Database.h"

// ===== INCLUDE IMPLEMENTATION FILES =====
#include "src/BST.cpp"
#include "src/KomikManager.cpp"
#include "src/MenuSystem.cpp"
#include "src/Auth.cpp"
#include "src/SearchFilter.cpp"
#include "src/Database.cpp"

#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>

using namespace std;

// ===== FUNCTION PROTOTYPES =====
void clearScreen();
void pause();
void printHeader(const string &title);
void crudKomikMenu(BST &tree, KomikManager &manager);
void crudGenreMenu(KomikManager &manager, BST &tree);
void crudAuthorMenu(KomikManager &manager, BST &tree);
void traversalMenu(BST &tree);
void viewFavoritesMenu(KomikManager &manager, BST &tree);

// Helper functions
vector<int> parseGenreChoices(const string &input);
string getMultipleGenres(KomikManager &manager, bool allowEmpty = false);
string getAuthorFromList(KomikManager &manager, bool allowEmpty = false);

// Global database connection
Database dbConnection;

void databaseInspectorMenu(Database &db, BST &tree)
{
    int choice;

    do
    {
        printHeader("DATABASE INSPECTOR");

        cout << "1. View Komiks Table\n";
        cout << "2. View Users Table\n";
        cout << "3. View Genres Table\n";
        cout << "4. View Authors Table\n";
        cout << "5. View Favorites Table\n";
        cout << "6. Database Statistics\n";
        cout << "7. Run Custom SQL Query\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("KOMIKS TABLE");
            vector<Komik *> komiks = db.getAllKomiks();

            if (komiks.empty())
            {
                cout << "\033[33mNo data in komiks table!\033[0m" << endl;
            }
            else
            {
                cout << left << setw(5) << "ID"
                     << setw(30) << "Title"
                     << setw(25) << "Author"
                     << setw(20) << "Genre(s)" << endl;
                cout << string(80, '-') << endl;

                for (Komik *k : komiks)
                {
                    cout << left << setw(5) << k->id
                         << setw(30) << k->title
                         << setw(25) << k->author
                         << setw(20) << k->genre << endl;
                }

                cout << "\nTotal records: " << komiks.size() << endl;
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("USERS TABLE");
            vector<User> users = db.getAllUsers();

            if (users.empty())
            {
                cout << "\033[33mNo data in users table!\033[0m" << endl;
            }
            else
            {
                cout << left << setw(5) << "ID"
                     << setw(20) << "Username"
                     << setw(40) << "Password (Hashed)"
                     << setw(10) << "Role" << endl;
                cout << string(75, '-') << endl;

                for (const User &u : users)
                {
                    cout << left << setw(5) << u.id
                         << setw(20) << u.username
                         << setw(40) << u.password.substr(0, 37) + "..."
                         << setw(10) << u.role << endl;
                }

                cout << "\nTotal records: " << users.size() << endl;
            }
            pause();
            break;
        }

        case 3:
        {
            printHeader("GENRES TABLE");
            vector<string> genres = db.getAllGenres();

            if (genres.empty())
            {
                cout << "\033[33mNo data in genres table!\033[0m" << endl;
            }
            else
            {
                cout << "Available Genres:\n\n";
                for (size_t i = 0; i < genres.size(); i++)
                {
                    cout << (i + 1) << ". " << genres[i] << endl;
                }

                cout << "\nTotal records: " << genres.size() << endl;
            }
            pause();
            break;
        }

        case 4:
        {
            printHeader("AUTHORS TABLE");
            vector<string> authors = db.getAllAuthors();

            if (authors.empty())
            {
                cout << "\033[33mNo data in authors table!\033[0m" << endl;
            }
            else
            {
                cout << "Available Authors:\n\n";
                for (size_t i = 0; i < authors.size(); i++)
                {
                    cout << (i + 1) << ". " << authors[i] << endl;
                }

                cout << "\nTotal records: " << authors.size() << endl;
            }
            pause();
            break;
        }

        case 5:
        {
            printHeader("FAVORITES TABLE");

            vector<User> users = db.getAllUsers();
            bool hasAnyFavorites = false;

            cout << left << setw(20) << "Username"
                 << setw(12) << "Komik ID"
                 << setw(35) << "Komik Title" << endl;
            cout << string(67, '-') << endl;

            for (const User &user : users)
            {
                vector<int> favs = db.getFavoritesByUser(user.username);

                if (!favs.empty())
                {
                    hasAnyFavorites = true;
                    for (int komikId : favs)
                    {
                        string title = "Unknown";
                        tree.inOrder([&title, komikId](Komik *k)
                                     {
                            if (k->id == komikId) {
                                title = k->title;
                            } });

                        cout << left << setw(20) << user.username
                             << setw(12) << komikId
                             << setw(35) << title << endl;
                    }
                }
            }

            if (!hasAnyFavorites)
            {
                cout << "\033[33mNo favorites in database!\033[0m" << endl;
            }

            pause();
            break;
        }

        case 6:
        {
            printHeader("DATABASE STATISTICS");

            vector<Komik *> komiks = db.getAllKomiks();
            vector<User> users = db.getAllUsers();
            vector<string> genres = db.getAllGenres();
            vector<string> authors = db.getAllAuthors();

            // Count total favorites
            int totalFavorites = 0;
            for (const User &user : users)
            {
                totalFavorites += db.getFavoritesByUser(user.username).size();
            }

            cout << "\033[1;36m╔══════════════════════════════════════╗\033[0m\n";
            cout << "\033[1;36m║      DATABASE STATISTICS             ║\033[0m\n";
            cout << "\033[1;36m╚══════════════════════════════════════╝\033[0m\n\n";

            cout << left << setw(25) << "Total Komiks:"
                 << "\033[32m" << komiks.size() << "\033[0m" << endl;
            cout << left << setw(25) << "Total Users:"
                 << "\033[32m" << users.size() << "\033[0m" << endl;
            cout << left << setw(25) << "Total Genres:"
                 << "\033[32m" << genres.size() << "\033[0m" << endl;
            cout << left << setw(25) << "Total Authors:"
                 << "\033[32m" << authors.size() << "\033[0m" << endl;
            cout << left << setw(25) << "Total Favorites:"
                 << "\033[32m" << totalFavorites << "\033[0m" << endl;

            cout << "\n\033[1;36m╔══════════════════════════════════════╗\033[0m\n";
            cout << "\033[1;36m║      USER BREAKDOWN                  ║\033[0m\n";
            cout << "\033[1;36m╚══════════════════════════════════════╝\033[0m\n\n";

            int adminCount = 0, userCount = 0;
            for (const User &u : users)
            {
                if (u.role == "admin")
                    adminCount++;
                else
                    userCount++;
            }

            cout << left << setw(25) << "Admins:"
                 << "\033[33m" << adminCount << "\033[0m" << endl;
            cout << left << setw(25) << "Regular Users:"
                 << "\033[33m" << userCount << "\033[0m" << endl;

            cout << "\n\033[36mDatabase file: komik_database.db\033[0m" << endl;

            pause();
            break;
        }

        case 7:
        {
            printHeader("RUN CUSTOM SQL QUERY");

            cout << "\033[33mWarning: Advanced feature!\033[0m\n";
            cout << "Only SELECT queries are safe.\n";
            cout << "Example: SELECT * FROM komiks WHERE genre LIKE '%Action%'\n\n";

            string query;
            cout << "Enter SQL query: ";
            getline(cin, query);

            // Simple validation (hanya allow SELECT)
            string queryLower = query;
            transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

            if (queryLower.find("select") != 0)
            {
                cout << "\033[31mError: Only SELECT queries are allowed!\033[0m" << endl;
                pause();
                break;
            }

            // Execute query (perlu tambah function di Database.cpp)
            cout << "\n\033[33mNote: Custom query execution not yet implemented.\033[0m" << endl;
            cout << "Use DB Browser for SQLite or sqlite3 CLI for custom queries." << endl;

            pause();
            break;
        }

        case 0:
            break;

        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

// ===== MAIN FUNCTION =====
int main()
{
    cout << "\033[1;36m";
    cout << "==============================================\n";
    cout << "   KOMIK MANAGEMENT SYSTEM\n";
    cout << "==============================================\033[0m\n"
         << endl;

    cout << "Initializing system...\n"
         << endl;

    // Connect to database
    cout << "Connecting to database...\n";
    if (!dbConnection.open("komik_database.db"))
    {
        cout << "\033[31mFailed to connect to database!\033[0m\n";
        return 1;
    }

    // Initialize managers
    KomikManager manager(&dbConnection);
    BST tree;
    Auth auth(&dbConnection);

    // Load data from database
    cout << "Loading data from database...\n";
    manager.loadDataFromDB(tree);

    cout << "\033[32mSystem initialized successfully!\033[0m\n"
         << endl;
    pause();

    // Start menu system
    MenuSystem menuSystem(tree, manager, auth);
    menuSystem.showMainMenu();

    // Close database connection
    dbConnection.close();

    cout << "\n\033[36mThank you for using Komik Management System!\033[0m" << endl;
    return 0;
}

// ===== HELPER FUNCTIONS =====

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string &title)
{
    clearScreen();
    cout << "\033[1;36m";
    cout << "==============================================\n";
    cout << "   " << title << "\n";
    cout << "==============================================\033[0m\n"
         << endl;
}

// ===== MULTIPLE GENRE HELPER FUNCTIONS =====

vector<int> parseGenreChoices(const string &input)
{
    vector<int> choices;
    stringstream ss(input);
    int num;

    while (ss >> num)
    {
        choices.push_back(num);
    }

    return choices;
}

string getMultipleGenres(KomikManager &manager, bool allowEmpty)
{
    vector<string> allGenres = manager.getAllGenres();

    cout << "\n\033[1;36mAvailable genres:\033[0m\n";
    for (size_t i = 0; i < allGenres.size(); i++)
    {
        cout << (i + 1) << ". " << allGenres[i] << endl;
    }

    vector<int> selectedIndices;
    string input;
    bool validInput = false;

    do
    {
        if (allowEmpty)
        {
            cout << "\nSelect genres (press Enter to keep current, or e.g., '1 3 5'): ";
        }
        else
        {
            cout << "\nSelect genres (separate with space, e.g., '1 3 5'): ";
        }
        getline(cin, input);

        selectedIndices = parseGenreChoices(input);

        if (selectedIndices.empty())
        {
            if (allowEmpty)
            {
                return "";
            }
            else
            {
                cout << "\033[31mError: Please select at least one genre!\033[0m" << endl;
                continue;
            }
        }

        bool allValid = true;
        vector<int> invalidNumbers;

        for (int idx : selectedIndices)
        {
            if (idx < 1 || idx > (int)allGenres.size())
            {
                allValid = false;
                invalidNumbers.push_back(idx);
            }
        }

        if (!allValid)
        {
            cout << "\033[31mError: Invalid genre number(s): ";
            for (size_t i = 0; i < invalidNumbers.size(); i++)
            {
                cout << invalidNumbers[i];
                if (i < invalidNumbers.size() - 1)
                    cout << ", ";
            }
            cout << "\033[0m" << endl;
            cout << "Valid range: 1-" << allGenres.size() << endl;
            continue;
        }

        validInput = true;

    } while (!validInput);

    sort(selectedIndices.begin(), selectedIndices.end());
    selectedIndices.erase(
        unique(selectedIndices.begin(), selectedIndices.end()),
        selectedIndices.end());

    string genreString = "";
    for (size_t i = 0; i < selectedIndices.size(); i++)
    {
        int idx = selectedIndices[i] - 1;
        genreString += allGenres[idx];
        if (i < selectedIndices.size() - 1)
        {
            genreString += ", ";
        }
    }

    cout << "\n\033[32mSelected genres: " << genreString << "\033[0m" << endl;
    return genreString;
}

// ===== HELPER: GET AUTHOR FROM LIST =====
string getAuthorFromList(KomikManager &manager, bool allowEmpty)
{
    vector<string> allAuthors = manager.getAllAuthors();

    cout << "\n\033[1;36mAvailable authors:\033[0m\n";
    for (size_t i = 0; i < allAuthors.size(); i++)
    {
        cout << (i + 1) << ". " << allAuthors[i] << endl;
    }

    // Opsi untuk add new author
    cout << (allAuthors.size() + 1) << ". \033[33m[Add New Author]\033[0m" << endl;

    int choice;
    bool validInput = false;
    string selectedAuthor = "";

    do
    {
        if (allowEmpty)
        {
            cout << "\nSelect author (press 0 to keep current, or 1-"
                 << (allAuthors.size() + 1) << "): ";
        }
        else
        {
            cout << "\nSelect author (1-" << (allAuthors.size() + 1) << "): ";
        }
        cin >> choice;

        // Error handling
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[31mInvalid input! Please enter a number.\033[0m" << endl;
            continue;
        }

        // Allow empty (untuk UPDATE)
        if (allowEmpty && choice == 0)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return "";
        }

        // Validasi range
        if (choice < 1 || choice > (int)(allAuthors.size() + 1))
        {
            cout << "\033[31mInvalid choice! Please select 1-"
                 << (allAuthors.size() + 1) << "\033[0m" << endl;
            continue;
        }

        // Kalau pilih "Add New Author"
        if (choice == (int)(allAuthors.size() + 1))
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nEnter new author name: ";
            getline(cin, selectedAuthor);

            if (selectedAuthor.empty())
            {
                cout << "\033[31mAuthor name cannot be empty!\033[0m" << endl;
                continue;
            }

            // Tambah author baru ke manager
            manager.addAuthor(selectedAuthor);
            validInput = true;
        }
        else
        {
            // Pilih dari list
            selectedAuthor = allAuthors[choice - 1];
            validInput = true;
        }

    } while (!validInput);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\033[32mSelected author: " << selectedAuthor << "\033[0m" << endl;

    return selectedAuthor;
}

// ===== CRUD KOMIK MENU =====
void crudKomikMenu(BST &tree, KomikManager &manager)
{
    int choice;

    do
    {
        printHeader("CRUD COMIC");
        cout << "Total comics in tree: " << tree.count() << "\n\n";
        cout << "1. Add Komik\n";
        cout << "2. View All Komiks (In-Order)\n";
        cout << "3. Search Komik\n";
        cout << "4. Update Komik\n";
        cout << "5. Delete Komik\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

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
            printHeader("ADD COMIC");

            string title, author, genre;

            // ===== INPUT TITLE =====
            cout << "Enter title: ";
            getline(cin, title);

            // ===== SELECT AUTHOR FROM LIST =====
            author = getAuthorFromList(manager, false);

            // ===== SELECT MULTIPLE GENRES =====
            genre = getMultipleGenres(manager, false);

            // ===== INSERT KE TREE =====
            int newId = manager.getNextId();
            Komik *newKomik = new Komik(newId, title, author, genre);
            tree.insert(newKomik);

            cout << "\n\033[32m==============================================\033[0m" << endl;
            cout << "\033[32mKomik added successfully!\033[0m" << endl;
            cout << "ID: " << newId << endl;
            cout << "Title: " << title << endl;
            cout << "Author: " << author << endl;
            cout << "Genre(s): " << genre << endl;
            cout << "\033[32m==============================================\033[0m" << endl;

            pause();
            break;
        }

        case 2:
        {
            printHeader("ALL COMICS (IN-ORDER TRAVERSAL)");

            if (tree.isEmpty())
            {
                cout << "\033[33mNo comics found!\033[0m" << endl;
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

                cout << "\nTotal: " << tree.count() << " comics" << endl;
            }

            pause();
            break;
        }

        case 3:
        {
            printHeader("SEARCH COMIC");

            string keyword;
            cout << "Enter keyword: ";
            getline(cin, keyword);

            vector<Komik *> results = tree.searchPartial(keyword);

            if (results.empty())
            {
                cout << "\033[31mNo comics found!\033[0m" << endl;
            }
            else
            {
                cout << "\033[32m\nFound " << results.size() << " comic(s):\033[0m\n"
                     << endl;

                cout << left << setw(5) << "ID"
                     << setw(30) << "Title"
                     << setw(25) << "Author"
                     << setw(20) << "Genre(s)" << endl;
                cout << string(80, '-') << endl;

                for (Komik *comic : results)
                {
                    cout << left << setw(5) << comic->id
                         << setw(30) << comic->title
                         << setw(25) << comic->author
                         << setw(20) << comic->genre << endl;
                }
            }

            pause();
            break;
        }

        case 4:
        {
            printHeader("UPDATE COMIC");

            string keyword;
            cout << "Enter keyword to find comic: ";
            getline(cin, keyword);

            Komik *found = tree.findByKeyword(keyword);

            if (found == nullptr)
            {
                pause();
                break;
            }

            cout << "\n\033[36mCurrent data:\033[0m\n";
            found->display();

            string newTitle, newAuthor, newGenre;

            cout << "\n\033[1;36mEnter new data:\033[0m\n";

            // ===== INPUT NEW TITLE =====
            cout << "Title: ";
            getline(cin, newTitle);

            // ===== SELECT NEW AUTHOR =====
            cout << "\n\033[36mCurrent author: " << found->author << "\033[0m";
            newAuthor = getAuthorFromList(manager, true);

            if (newAuthor.empty())
            {
                newAuthor = found->author;
                cout << "\033[33mKeeping current author.\033[0m" << endl;
            }

            // ===== SELECT NEW GENRES =====
            cout << "\n\033[36mCurrent genre(s): " << found->genre << "\033[0m";
            newGenre = getMultipleGenres(manager, true);

            if (newGenre.empty())
            {
                newGenre = found->genre;
                cout << "\033[33mKeeping current genre(s).\033[0m" << endl;
            }

            // ===== UPDATE TREE =====
            string oldTitle = found->title;

            Komik *updatedKomik = new Komik(found->id, newTitle, newAuthor, newGenre);
            tree.update(oldTitle, updatedKomik);

            cout << "\n\033[32mKomik updated successfully!\033[0m" << endl;
            pause();
            break;
        }

        case 5:
        {
            printHeader("DELETE COMIC");

            string keyword;
            cout << "Enter keyword to find comic: ";
            getline(cin, keyword);

            // Pakai helper! Otomatis handle multiple results
            Komik *found = tree.findByKeyword(keyword);

            if (found == nullptr)
            {
                pause();
                break;
            }

            cout << "\n\033[33mAre you sure you want to delete this comic?\033[0m\n";
            found->display();
            cout << "\n(y/n): ";

            char confirm;
            cin >> confirm;
            cin.ignore();

            if (confirm == 'y' || confirm == 'Y')
            {
                tree.remove(found->title); // Pakai title dari found
                cout << "\033[32mKomik deleted successfully!\033[0m" << endl;
            }
            else
            {
                cout << "Delete cancelled." << endl;
            }

            pause();
            break;
        }

        case 0:
            break;

        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

// ===== CRUD GENRE MENU =====
void crudGenreMenu(KomikManager &manager, BST &tree)
{
    int choice;

    do
    {
        printHeader("CRUD GENRE");

        cout << "1. View All Genres\n";
        cout << "2. Add Genre\n";
        cout << "3. Update Genre\n";
        cout << "4. Delete Genre\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("ALL GENRES");
            vector<string> genres = manager.getAllGenres();

            if (genres.empty())
            {
                cout << "\033[33mNo genres found!\033[0m" << endl;
            }
            else
            {
                cout << "Total genres: " << genres.size() << "\n\n";
                for (size_t i = 0; i < genres.size(); i++)
                {
                    cout << (i + 1) << ". " << genres[i] << endl;
                }
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("ADD GENRE");
            string name;
            cout << "Enter genre name: ";
            getline(cin, name);
            manager.addGenre(name);
            pause();
            break;
        }

        case 3:
        {
            printHeader("UPDATE GENRE");
            string oldName, newName;
            cout << "Enter old genre name: ";
            getline(cin, oldName);
            cout << "Enter new genre name: ";
            getline(cin, newName);
            manager.updateGenre(oldName, newName, tree);
            pause();
            break;
        }

        case 4:
        {
            printHeader("DELETE GENRE");
            string name;
            cout << "Enter genre name to delete: ";
            getline(cin, name);

            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                manager.deleteGenre(name);
            }
            pause();
            break;
        }
        }
    } while (choice != 0);
}

// ===== CRUD AUTHOR MENU =====
void crudAuthorMenu(KomikManager &manager, BST &tree)
{
    int choice;

    do
    {
        printHeader("CRUD AUTHOR");

        cout << "1. View All Authors\n";
        cout << "2. Add Author\n";
        cout << "3. Update Author\n";
        cout << "4. Delete Author\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("ALL AUTHORS");
            vector<string> authors = manager.getAllAuthors();

            if (authors.empty())
            {
                cout << "\033[33mNo authors found!\033[0m" << endl;
            }
            else
            {
                cout << "Total authors: " << authors.size() << "\n\n";
                for (size_t i = 0; i < authors.size(); i++)
                {
                    cout << (i + 1) << ". " << authors[i] << endl;
                }
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("ADD AUTHOR");
            string name;
            cout << "Enter author name: ";
            getline(cin, name);
            manager.addAuthor(name);
            pause();
            break;
        }

        case 3:
        {
            printHeader("UPDATE AUTHOR");
            string oldName, newName;
            cout << "Enter old author name: ";
            getline(cin, oldName);
            cout << "Enter new author name: ";
            getline(cin, newName);
            manager.updateAuthor(oldName, newName, tree);
            pause();
            break;
        }

        case 4:
        {
            printHeader("DELETE AUTHOR");
            string name;
            cout << "Enter author name to delete: ";
            getline(cin, name);

            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                manager.deleteAuthor(name);
            }
            pause();
            break;
        }
        }
    } while (choice != 0);
}

// ===== TRAVERSAL MENU =====
void traversalMenu(BST &tree)
{
    int choice;

    do
    {
        printHeader("TREE TRAVERSAL");

        cout << "1. Pre-Order Traversal\n";
        cout << "2. In-Order Traversal\n";
        cout << "3. Post-Order Traversal\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            printHeader("PRE-ORDER TRAVERSAL");
            cout << "Order: Root -> Left -> Right\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                tree.preOrder([](Komik *comic)
                              { comic->display(); });
            }
            pause();
            break;

        case 2:
            printHeader("IN-ORDER TRAVERSAL");
            cout << "Order: Left -> Root -> Right (Sorted)\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                tree.inOrder([](Komik *comic)
                             { comic->display(); });
            }
            pause();
            break;

        case 3:
            printHeader("POST-ORDER TRAVERSAL");
            cout << "Order: Left -> Right -> Root\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                tree.postOrder([](Komik *comic)
                               { comic->display(); });
            }
            pause();
            break;
        }
    } while (choice != 0);
}

// ===== VIEW FAVORITES MENU =====
void viewFavoritesMenu(KomikManager &manager, BST &tree)
{
    printHeader("ALL USER FAVORITES");

    // Get all users from database
    vector<User> allUsers = dbConnection.getAllUsers();

    if (allUsers.empty())
    {
        cout << "\033[33mNo users found!\033[0m" << endl;
    }
    else
    {
        bool hasAnyFavorites = false;

        cout << left << setw(15) << "Username"
             << setw(10) << "Komik ID"
             << setw(30) << "Komik Title" << endl;
        cout << string(55, '-') << endl;

        for (const User &user : allUsers)
        {
            vector<int> favorites = manager.getFavoritesByUser(user.username);

            if (!favorites.empty())
            {
                hasAnyFavorites = true;
                for (int comicId : favorites)
                {
                    string comicTitle = "Unknown";

                    tree.inOrder([&comicTitle, comicId](Komik *comic)
                                 {
                        if (comic->id == comicId) {
                            comicTitle = comic->title;
                        } });

                    cout << left << setw(15) << user.username
                         << setw(10) << comicId
                         << setw(30) << comicTitle << endl;
                }
            }
        }

        if (!hasAnyFavorites)
        {
            cout << "\033[33mNo favorites found!\033[0m" << endl;
        }
    }

    pause();
}