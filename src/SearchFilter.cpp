#include "../include/SearchFilter.h"
#include <algorithm>   // transform

// ===================== toLower() =====================
string SearchFilter::toLower(const string& str) const {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}



// ===================== SEARCH BY TITLE =====================
vector<Komik*> SearchFilter::searchByTitle(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string titleLower = toLower(comic->title);
        if (titleLower.find(key) != string::npos) {
            result.push_back(comic);
        }
    });

    return result;
}



// ===================== SEARCH BY GENRE =====================
vector<Komik*> SearchFilter::searchByGenre(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string genreLower = toLower(comic->genre);
        if (genreLower.find(key) != string::npos) {
            result.push_back(comic);
        }
    });

    return result;
}



// ===================== SEARCH BY AUTHOR =====================
vector<Komik*> SearchFilter::searchByAuthor(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string authorLower = toLower(comic->author);
        if (authorLower.find(key) != string::npos) {
            result.push_back(comic);
        }
    });

    return result;
}
