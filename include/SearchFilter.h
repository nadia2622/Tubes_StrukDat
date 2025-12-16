#ifndef SEARCH_FILTER_H
#define SEARCH_FILTER_H

#include <string>
#include <vector>
#include <algorithm>
#include "Komik.h"
#include "BST.h"

using namespace std;

class SearchFilter
{
public:
    vector<Komik *> searchByTitle(const BST &tree, const string &keyword) const;
    vector<Komik *> searchByGenre(const BST &tree, const string &keyword) const;
    vector<Komik *> searchByAuthor(const BST &tree, const string &keyword) const;

private:
    string toLower(const string &str) const;
};

#endif // SEARCH_FILTER_H
