#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "Komik.h"

using namespace std;

// Forward declaration
struct User;

class Database
{
private:
    sqlite3 *db;
    string dbPath;

    bool createTables();

public:
    Database();
    ~Database();

    // Connection
    bool open(const string &path);
    void close();
    bool isOpen() const;

    // Komik Operations
    bool insertKomik(Komik *komik);
    bool updateKomik(int id, Komik *komik);
    bool deleteKomik(int id);
    vector<Komik *> getAllKomiks();
    Komik *getKomikById(int id);
    int getLastKomikId();

    // Genre Operations
    bool insertGenre(const string &name);
    bool updateGenre(const string &oldName, const string &newName);
    bool deleteGenre(const string &name);
    vector<string> getAllGenres();
    bool genreExists(const string &name);

    // Author Operations
    bool insertAuthor(const string &name);
    bool updateAuthor(const string &oldName, const string &newName);
    bool deleteAuthor(const string &name);
    vector<string> getAllAuthors();
    bool authorExists(const string &name);

    // User Operations
    bool insertUser(const User &user);
    bool updateUser(const User &user);
    bool deleteUser(const string &username);
    vector<User> getAllUsers();
    User *getUserByUsername(const string &username);
    bool userExists(const string &username);
    int getLastUserId();

    // Favorites Operations
    bool addFavorite(const string &username, int komikId);
    bool removeFavorite(const string &username, int komikId);
    vector<int> getFavoritesByUser(const string &username);
    bool isFavorite(const string &username, int komikId);
};

#endif