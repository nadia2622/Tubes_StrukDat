#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <string>
#include <vector>
#include "BST.h"
#include "Komik.h"

using namespace std;

class SearchFilter {
private:
    // Helper methods (Internal)
    string toLowerCase(const string& str);
    bool containsIgnoreCase(const string& str1, const string& str2);
    void dfsSearchHelper(Komik* node, const string& title, vector<Komik*>& results);

public:
    // Constructor & Destructor
    SearchFilter();
    ~SearchFilter();

    // Main Search & Filter methods
    vector<Komik*> searchByTitle(BST& tree, const string& title);
    vector<Komik*> searchByAuthor(BST& tree, const string& author);
    vector<Komik*> searchByGenre(BST& tree, const string& genre);
    
    // Method tambahan untuk filter spesifik
    vector<Komik*> filterByGenre(BST& tree, const string& genre);
    vector<Komik*> filterByAuthor(BST& tree, const string& author);
    vector<Komik*> filterByMultiple(BST& tree, const string& genre, const string& author);

    // Display
    void displayResults(const vector<Komik*>& results, const string& message);
};

#endif