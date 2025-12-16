#ifndef KOMIKMANAGER_H
#define KOMIKMANAGER_H

#include <vector>
#include <string>
#include <map>
#include "Komik.h"
#include "BST.h"
#include "Database.h"

using namespace std;

class KomikManager
{
private:
    vector<string> genres;
    vector<string> authors;
    int nextId;
    Database *db;

public:
    KomikManager(Database *database);

    // Load
    void loadDataFromDB(BST &tree);

    // ID
    int getNextId();

    // Genre
    void addGenre(const string &name);
    void deleteGenre(const string &name);
    vector<string> getAllGenres();
    void updateGenre(const string &oldName, const string &newName, BST &tree);

    // Author
    void addAuthor(const string &name);
    void deleteAuthor(const string &name);
    vector<string> getAllAuthors();
    void updateAuthor(const string &oldName, const string &newName, BST &tree);

    // Favorites
    void addFavorite(const string &username, int comicId);
    void removeFavorite(const string &username, int comicId);
    vector<int> getFavoritesByUser(const string &username);

    // Helper sample data (dikososngkan saja)
    void loadSampleData(BST &tree) {}
};

#endif