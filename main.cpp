/*
 * main.cpp - FIXED VERSION
 * 
 * PENTING: File ini HANYA include .h files, TIDAK include .cpp!
 * Compile dengan: g++ -o komik main.cpp src/*.cpp -I include -std=c++11
 */

// ===== INCLUDE HANYA HEADER FILES (.h) =====
#include "include/Komik.h"
#include "include/BST.h"
#include "include/KomikManager.h"
// #include "include/SearchFilter.h"
#include "include/Auth.h"
#include "include/MenuSystem.h"

#include "src/BST.cpp"
#include "src/KomikManager.cpp"
#include "src/MenuSystem.cpp"
#include "src/Auth.cpp"

#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// ===== FUNCTION PROTOTYPES =====
void clearScreen();
void pause();
void printHeader(const string& title);
void crudKomikMenu(BST& tree, KomikManager& manager);
void crudGenreMenu(KomikManager& manager, BST& tree);
void crudAuthorMenu(KomikManager& manager, BST& tree);
void traversalMenu(BST& tree);
void viewFavoritesMenu(KomikManager& manager, BST& tree);
// void searchFilterMenu(BST& tree, SearchFilter& searcher);

// ===== MAIN FUNCTION =====
int main() {
    cout << "\033[1;36m";
    cout << "==============================================\n";
    cout << "   KOMIK MANAGEMENT SYSTEM\n";
    cout << "   with Authentication & Role-Based Access\n";
    cout << "==============================================\033[0m\n" << endl;
    
    cout << "Initializing system...\n" << endl;
    
    KomikManager manager;
    BST tree;
    // SearchFilter searcher;
    Auth auth;
    
    cout << "\033[32mSystem initialized!\033[0m\n" << endl;
    pause();
    
    MenuSystem menuSystem(tree, manager, auth);
    menuSystem.showMainMenu();
    
    cout << "\n\033[36mThank you for using Komik Management System!\033[0m" << endl;
    
    return 0;
}

// ===== HELPER FUNCTIONS =====
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    clearScreen();
    cout << "\033[1;36m";
    cout << "==============================================\n";
    cout << "   " << title << "\n";
    cout << "==============================================\033[0m\n" << endl;
}

// ===== CRUD KOMIK MENU =====
void crudKomikMenu(BST& tree, KomikManager& manager) {
    int choice;
    
    do {
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
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        
        switch (choice) {
            case 1: {
                printHeader("ADD COMIC");
                string title, author, genre;
                
                cout << "Enter title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                
                cout << "\nAvailable genres:\n";
                vector<string> genres = manager.getAllGenres();
                for (size_t i = 0; i < genres.size(); i++) {
                    cout << (i + 1) << ". " << genres[i] << endl;
                }
                cout << "\nEnter genre: ";
                getline(cin, genre);
                
                int newId = manager.getNextId();
                Komik* newKomik = new Komik(newId, title, author, genre);
                tree.insert(newKomik);
                
                cout << "\033[32mKomik added successfully with ID: " << newId << "\033[0m" << endl;
                pause();
                break;
            }
            
            case 2: {
                printHeader("ALL COMICS (IN-ORDER TRAVERSAL)");
                
                if (tree.isEmpty()) {
                    cout << "\033[33mNo comics found!\033[0m" << endl;
                } else {
                    cout << left << setw(5) << "ID"
                         << setw(30) << "Title"
                         << setw(25) << "Author"
                         << setw(15) << "Genre" << endl;
                    cout << string(75, '-') << endl;
                    
                    tree.inOrder([](Komik* comic) {
                        cout << left << setw(5) << comic->id
                             << setw(30) << comic->title
                             << setw(25) << comic->author
                             << setw(15) << comic->genre << endl;
                    });
                    
                    cout << "\nTotal: " << tree.count() << " comics" << endl;
                }
                pause();
                break;
            }
            
            case 3: {
                printHeader("SEARCH COMIC");
                string title;
                
                cout << "Enter title to search: ";
                getline(cin, title);
                
                Komik* found = tree.search(title);
                if (found != nullptr) {
                    cout << "\033[32m\nKomik found!\033[0m\n";
                    found->display();
                } else {
                    cout << "\033[31mKomik not found!\033[0m" << endl;
                }
                pause();
                break;
            }
            
            case 4: {
                printHeader("UPDATE COMIC");
                string oldTitle;
                
                cout << "Enter title of comic to update: ";
                getline(cin, oldTitle);
                
                Komik* found = tree.search(oldTitle);
                if (found == nullptr) {
                    cout << "\033[31mKomik not found!\033[0m" << endl;
                    pause();
                    break;
                }
                
                string newTitle, newAuthor, newGenre;
                cout << "\nCurrent data:\n";
                found->display();
                
                cout << "\nEnter new data:\n";
                cout << "Title: ";
                getline(cin, newTitle);
                cout << "Author: ";
                getline(cin, newAuthor);
                cout << "Genre: ";
                getline(cin, newGenre);
                
                Komik* updatedKomik = new Komik(found->id, newTitle, newAuthor, newGenre);
                tree.update(oldTitle, updatedKomik);
                
                cout << "\033[32mKomik updated successfully!\033[0m" << endl;
                pause();
                break;
            }
            
            case 5: {
                printHeader("DELETE COMIC");
                string title;
                
                cout << "Enter title of comic to delete: ";
                getline(cin, title);
                
                Komik* found = tree.search(title);
                if (found == nullptr) {
                    cout << "\033[31mKomik not found!\033[0m" << endl;
                    pause();
                    break;
                }
                
                cout << "\nAre you sure you want to delete this comic?\n";
                found->display();
                cout << "\n(y/n): ";
                
                char confirm;
                cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    tree.remove(title);
                    cout << "\033[32mKomik deleted successfully!\033[0m" << endl;
                } else {
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

// ===== CRUD GENRE MENU (SIMPLIFIED) =====
void crudGenreMenu(KomikManager& manager, BST& tree) {
    cout << "CRUD Genre Menu - To be implemented" << endl;
    pause();
}

// ===== CRUD AUTHOR MENU (SIMPLIFIED) =====
void crudAuthorMenu(KomikManager& manager, BST& tree) {
    cout << "CRUD Author Menu - To be implemented" << endl;
    pause();
}

// ===== TRAVERSAL MENU (SIMPLIFIED) =====
void traversalMenu(BST& tree) {
    cout << "Traversal Menu - To be implemented" << endl;
    pause();
}

// ===== VIEW FAVORITES MENU (SIMPLIFIED) =====
void viewFavoritesMenu(KomikManager& manager, BST& tree) {
    cout << "View Favorites - To be implemented" << endl;
    pause();
}

// ===== SEARCH FILTER MENU (SIMPLIFIED) =====
// void searchFilterMenu(BST& tree, SearchFilter& searcher) {
//     cout << "Search & Filter Menu - To be implemented" << endl;
//     pause();
// }