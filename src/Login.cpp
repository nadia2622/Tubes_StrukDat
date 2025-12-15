/*
 * Login.cpp
 * File IMPLEMENTASI untuk Class Login
 *
 * Class ini menangani tampilan menu login dan input kredensial.
 */

#include "../include/Login.h"
#include <iostream>
#include <limits>

using namespace std;

// ===== CONSTRUCTOR =====
Login::Login()
{
    // Constructor kosong, tidak ada inisialisasi khusus
}

// ===== DESTRUCTOR =====
Login::~Login()
{
    // Destructor kosong
}

// ===== PRINT LOGIN HEADER =====
// Fungsi untuk print header dengan border
void Login::printLoginHeader(const string &title)
{
// Clear screen
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // Print border dan title
    cout << "\033[1;36m"; // Warna cyan + bold
    cout << "==============================================\n";
    cout << "   " << title << "\n";
    cout << "==============================================\033[0m\n"
         << endl;
}

// ===== SHOW LOGIN MENU =====
// Tampilkan menu login: 1. Login, 2. Register, 0. Exit
// Return: pilihan user (int)
int Login::showLoginMenu()
{
    printLoginHeader("WELCOME TO KOMIK MANAGEMENT SYSTEM");

    cout << "1. Login\n";
    cout << "2. Register (New User)\n";
    cout << "0. Exit\n";
    cout << "\nChoice: ";

    int choice;
    cin >> choice;

    // Error handling jika input bukan angka
    if (cin.fail())
    {
        cin.clear();                                         // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input salah
        return -1;                                           // Return -1 untuk invalid
    }

    cin.ignore(); // Abaikan newline setelah cin >>
    return choice;
}

// ===== INPUT CREDENTIALS =====
// Input username dan password dari user
void Login::inputCredentials(string &username, string &password)
{
    printLoginHeader("LOGIN");

    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);
}

// ===== SHOW REGISTER FORM =====
// Tampilkan form registrasi untuk user baru
void Login::showRegisterForm(string &username, string &password)
{
    printLoginHeader("REGISTER NEW ACCOUNT");

    cout << "\033[33mNote: Registered accounts will have 'User' role.\033[0m\n\n";

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);
}
