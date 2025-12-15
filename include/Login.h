/*
 * Login.h
 * File DEKLARASI untuk Class Login
 *
 * Class ini menangani tampilan menu login dan input kredensial.
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <iostream>

using namespace std;

// ===== STRUCT USER =====
// Menyimpan data user: id, username, password, role
struct User
{
    int id;
    string username;
    string password;
    string role; // "admin" atau "user"

    // Pointer untuk linked list (opsional, untuk struktur data)
    User *next;

    // ===== DEFAULT CONSTRUCTOR =====
    User()
    {
        id = 0;
        username = "";
        password = "";
        role = "user";
        next = nullptr;
    }

    // ===== PARAMETERIZED CONSTRUCTOR =====
    User(int id_input, string username_input, string password_input, string role_input)
    {
        id = id_input;
        username = username_input;
        password = password_input;
        role = role_input;
        next = nullptr;
    }

    // ===== DESTRUCTOR =====
    ~User() {}

    // ===== DISPLAY USER INFO =====
    void display() const
    {
        cout << "ID: " << id
             << " | Username: " << username
             << " | Role: " << role << endl;
    }
};

// ===== CLASS LOGIN =====
// Class untuk menampilkan menu login dan menangani input
class Login
{
public:
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    Login();
    ~Login();

    // ===== PUBLIC METHODS =====

    // Tampilkan menu login (1. Login, 2. Register, 0. Exit)
    int showLoginMenu();

    // Input username dan password dari user
    // Parameter: username dan password (passed by reference)
    void inputCredentials(string &username, string &password);

    // Tampilkan form registrasi
    // Parameter: username dan password (passed by reference)
    void showRegisterForm(string &username, string &password);

    // Tampilkan header dengan judul
    void printLoginHeader(const string &title);
};

#endif // LOGIN_H
