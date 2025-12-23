#ifndef BST_H
#define BST_H

#include "Komik.h"
#include <functional>
#include <vector>

using namespace std;

class BST
{
private:
    Komik *root;

    Komik *insertHelper(Komik *node, Komik *newKomik);
    Komik *searchHelper(Komik *node, const string &title) const;
    Komik *deleteHelper(Komik *node, const string &title);
    Komik *findMin(Komik *node) const;

    void preOrderHelper(Komik *node, function<void(Komik *)> callback) const;
    void inOrderHelper(Komik *node, function<void(Komik *)> callback) const;
    void postOrderHelper(Komik *node, function<void(Komik *)> callback) const;

    void destroyTree(Komik *node);
    int countHelper(Komik *node) const;

    // NEW: Helper untuk partial search
    void searchPartialHelper(Komik *node, const string &keyword, vector<Komik *> &results) const;
    string toLowerCase(const string &str) const;

public:
    BST();
    ~BST();

    void insert(Komik *newKomik);
    Komik *search(const string &title) const;

    // NEW: Partial search (case-insensitive)
    Komik* findByKeyword(const string& keyword) const;
    vector<Komik *> searchPartial(const string &keyword) const;

    bool remove(const string &title);
    bool update(const string &oldTitle, Komik *updatedKomik);

    void preOrder(function<void(Komik *)> callback) const;
    void inOrder(function<void(Komik *)> callback) const;
    void postOrder(function<void(Komik *)> callback) const;

    bool isEmpty() const;
    int count() const;
    void clear();
    vector<Komik *> getAllKomiks() const;

    Komik *getRoot() const { return root; }
};

#endif