#ifndef KOMIK_MANAGER_H
#define KOMIK_MANAGER_H

#include "BST.h"
#include <string>
#include <vector>
#include <map>

class KomikManager {
private:
    int nextKomikId;
    
    vector<string> genres;
    vector<string> authors;
    
    map<string, vector<int>> userFavorites;

public:
    KomikManager();
    ~KomikManager();

    void loadSampleData(BST& tree);
    
    int getNextId();

    // Genre
    vector<string> getAllGenres() const;
    bool addGenre(const string& name);
    bool updateGenre(const string& oldName, const string& newName, BST& tree);
    bool deleteGenre(const string& name);
    bool genreExists(const string& name) const;
    
    // Author
    vector<string> getAllAuthors() const;
    bool addAuthor(const string& name);
    bool updateAuthor(const string& oldName, const string& newName, BST& tree);
    bool deleteAuthor(const string& name);
    bool authorExists(const string& name) const;

    // Favorite
    void addFavorite(const string& username, int komikId);
    bool removeFavorite(const string& username, int komikId);
    vector<int> getFavoritesByUser(const string& username) const;
    
    map<string, vector<int>> getAllFavorites() const;
    
    void reset();
};

#endif // KOMIK_MANAGER_H