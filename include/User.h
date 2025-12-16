#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

struct User {
    int id;
    string username;
    string password;
    string role; // "admin" or "user"
};
#endif