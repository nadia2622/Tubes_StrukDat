#include "../include/SearchFilter.h"
#include <algorithm>   // transform

// ===================== toLower() =====================
// Mengubah string menjadi huruf kecil seluruhnya
string SearchFilter::toLower(const string& str) const {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}



// ===================== SEARCH BY TITLE (Fuzzy Search) =====================
// Melakukan pencarian komik berdasarkan judul (tidak harus sama persis)
// Contoh: input "nar" akan menemukan "Naruto Shippuden"
vector<Komik*> SearchFilter::searchByTitle(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string titleLower = toLower(comic->title);
        if (titleLower.find(key) != string::npos) {    // mengecek apakah keyword ada dalam title
            result.push_back(comic);
        }
    });

    return result;
}



// ===================== FILTER BY GENRE =====================
// Menyaring komik berdasarkan genre (tidak harus sama persis)
// Contoh: input "act" akan menemukan "Action", "Action-Adventure", dll.
vector<Komik*> SearchFilter::searchByGenre(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string genreLower = toLower(comic->genre);
        if (genreLower.find(key) != string::npos) {    // fuzzy / contains
            result.push_back(comic);
        }
    });

    return result;
}



// ===================== FILTER BY AUTHOR =====================
// Menyaring komik berdasarkan nama author (tidak harus sama persis)
// Contoh: input "oda" akan menemukan "Eiichiro Oda"
vector<Komik*> SearchFilter::searchByAuthor(const BST& tree, const string& keyword) const {
    vector<Komik*> result;
    string key = toLower(keyword);

    tree.inOrder([&](Komik* comic) {
        string authorLower = toLower(comic->author);
        if (authorLower.find(key) != string::npos) {   // fuzzy / contains
            result.push_back(comic);
        }
    });

    return result;
}
