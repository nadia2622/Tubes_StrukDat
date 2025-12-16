/*
 * KomikManager.cpp
 * File IMPLEMENTASI Komik Manager dengan SQLite Database
 */

#include "../include/KomikManager.h"
#include "../include/Database.h"
#include <iostream>
#include <algorithm>

using namespace std;

// ===== CONSTRUCTOR =====
KomikManager::KomikManager(Database *database) : db(database)
{
    if (db)
    {
        nextId = db->getLastKomikId() + 1;

        // Load Genre dari database
        genres = db->getAllGenres();
        if (genres.empty())
        {
            // Insert default genres
            db->insertGenre("Action");
            db->insertGenre("Comedy");
            db->insertGenre("Horror");
            db->insertGenre("Romance");
            db->insertGenre("Fantasy");
            db->insertGenre("Mystery");
            db->insertGenre("Sci-Fi");
            db->insertGenre("Drama");
            genres = db->getAllGenres();
        }

        // Load Authors dari database
        authors = db->getAllAuthors();
    }
    else
    {
        nextId = 1;
    }
}

// ===== LOAD DATA FROM DB =====
void KomikManager::loadDataFromDB(BST &tree)
{
    if (!db)
        return;

    vector<Komik *> data = db->getAllKomiks();
    for (Komik *k : data)
        tree.insert(k);

    cout << "Loaded " << data.size() << " comics from database." << endl;
}

// ===== GET NEXT ID =====
int KomikManager::getNextId()
{
    return nextId++;
}

// ===== GENRE OPERATIONS =====

vector<string> KomikManager::getAllGenres()
{
    if (db)
    {
        genres = db->getAllGenres();
    }
    return genres;
}

void KomikManager::addGenre(const string &name)
{
    // Check if exists in memory
    if (find(genres.begin(), genres.end(), name) != genres.end())
    {
        cerr << "Genre '" << name << "' already exists!" << endl;
        return;
    }

    // Add to database
    if (db && db->insertGenre(name))
    {
        genres.push_back(name);
        cout << "Genre '" << name << "' added successfully!" << endl;
    }
    else
    {
        cerr << "Failed to add genre to database!" << endl;
    }
}

void KomikManager::updateGenre(const string &oldName, const string &newName, BST &tree)
{
    // Check if old genre exists
    auto it = find(genres.begin(), genres.end(), oldName);
    if (it == genres.end())
    {
        cerr << "Genre '" << oldName << "' not found!" << endl;
        return;
    }

    // Check if new name already exists
    if (oldName != newName && find(genres.begin(), genres.end(), newName) != genres.end())
    {
        cerr << "Genre '" << newName << "' already exists!" << endl;
        return;
    }

    // Update in database
    if (db && db->updateGenre(oldName, newName))
    {
        // Update in memory
        *it = newName;

        // Update all komiks with this genre
        vector<Komik *> allKomiks = tree.getAllKomiks();
        for (Komik *comic : allKomiks)
        {
            if (comic->genre.find(oldName) != string::npos)
            {
                // Replace oldName with newName in genre string
                size_t pos = comic->genre.find(oldName);
                while (pos != string::npos)
                {
                    comic->genre.replace(pos, oldName.length(), newName);
                    pos = comic->genre.find(oldName, pos + newName.length());
                }

                // Update in database
                db->updateKomik(comic->id, comic);
            }
        }

        cout << "Genre updated successfully!" << endl;
    }
    else
    {
        cerr << "Failed to update genre in database!" << endl;
    }
}

void KomikManager::deleteGenre(const string &name)
{
    auto it = find(genres.begin(), genres.end(), name);
    if (it == genres.end())
    {
        cerr << "Genre '" << name << "' not found!" << endl;
        return;
    }

    // Delete from database
    if (db && db->deleteGenre(name))
    {
        genres.erase(it);
        cout << "Genre '" << name << "' deleted successfully!" << endl;
    }
    else
    {
        cerr << "Failed to delete genre from database!" << endl;
    }
}

// ===== AUTHOR OPERATIONS =====

vector<string> KomikManager::getAllAuthors()
{
    if (db)
    {
        authors = db->getAllAuthors();
    }
    return authors;
}

void KomikManager::addAuthor(const string &name)
{
    if (find(authors.begin(), authors.end(), name) != authors.end())
    {
        cerr << "Author '" << name << "' already exists!" << endl;
        return;
    }

    if (db && db->insertAuthor(name))
    {
        authors.push_back(name);
        cout << "Author '" << name << "' added successfully!" << endl;
    }
    else
    {
        cerr << "Failed to add author to database!" << endl;
    }
}

void KomikManager::updateAuthor(const string &oldName, const string &newName, BST &tree)
{
    auto it = find(authors.begin(), authors.end(), oldName);
    if (it == authors.end())
    {
        cerr << "Author '" << oldName << "' not found!" << endl;
        return;
    }

    if (oldName != newName && find(authors.begin(), authors.end(), newName) != authors.end())
    {
        cerr << "Author '" << newName << "' already exists!" << endl;
        return;
    }

    if (db && db->updateAuthor(oldName, newName))
    {
        *it = newName;

        // Update all komiks with this author
        vector<Komik *> allKomiks = tree.getAllKomiks();
        for (Komik *comic : allKomiks)
        {
            if (comic->author == oldName)
            {
                comic->author = newName;
                db->updateKomik(comic->id, comic);
            }
        }

        cout << "Author updated successfully!" << endl;
    }
    else
    {
        cerr << "Failed to update author in database!" << endl;
    }
}

void KomikManager::deleteAuthor(const string &name)
{
    auto it = find(authors.begin(), authors.end(), name);
    if (it == authors.end())
    {
        cerr << "Author '" << name << "' not found!" << endl;
        return;
    }

    if (db && db->deleteAuthor(name))
    {
        authors.erase(it);
        cout << "Author '" << name << "' deleted successfully!" << endl;
    }
    else
    {
        cerr << "Failed to delete author from database!" << endl;
    }
}

// ===== FAVORITES OPERATIONS =====

void KomikManager::addFavorite(const string &username, int comicId)
{
    if (!db)
    {
        cerr << "Database not available!" << endl;
        return;
    }

    // Check if already favorite
    if (db->isFavorite(username, comicId))
    {
        cout << "Komik already in favorites!" << endl;
        return;
    }

    // Add to database
    if (db->addFavorite(username, comicId))
    {
        cout << "Komik added to favorites!" << endl;
    }
    else
    {
        cerr << "Failed to add to favorites!" << endl;
    }
}

void KomikManager::removeFavorite(const string &username, int comicId)
{
    if (!db)
    {
        cerr << "Database not available!" << endl;
        return;
    }

    if (db->removeFavorite(username, comicId))
    {
        cout << "Komik removed from favorites!" << endl;
    }
    else
    {
        cerr << "Failed to remove from favorites!" << endl;
    }
}

vector<int> KomikManager::getFavoritesByUser(const string &username)
{
    if (db)
    {
        return db->getFavoritesByUser(username);
    }
    return vector<int>();
}