#include "../include/SearchFilter.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

// ===== CONSTRUCTOR =====
SearchFilter::SearchFilter()
{
    // No initialization needed
}

// ===== DESTRUCTOR =====
SearchFilter::~SearchFilter()
{
    // No cleanup needed
}

// ===== HELPER: TO LOWERCASE =====
string SearchFilter::toLowerCase(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// ===== HELPER: CONTAINS IGNORE CASE =====
bool SearchFilter::containsIgnoreCase(const string &str1, const string &str2)
{
    string lower1 = toLowerCase(str1);
    string lower2 = toLowerCase(str2);
    return lower1.find(lower2) != string::npos;
}

// ===== HELPER: DFS SEARCH =====
void SearchFilter::dfsSearchHelper(Komik *node, const string &title, vector<Komik *> &results)
{
    if (node == nullptr)
        return;

    // Check current node
    if (containsIgnoreCase(node->title, title))
    {
        results.push_back(node);
    }

    // Search left subtree
    dfsSearchHelper(node->left, title, results);

    // Search right subtree
    dfsSearchHelper(node->right, title, results);
}

// ===== SEARCH BY TITLE (DFS) =====
vector<Komik *> SearchFilter::searchByTitle(BST &tree, const string &title)
{
    vector<Komik *> results;

    if (title.empty())
    {
        cout << "\033[31mError: Title cannot be empty!\033[0m" << endl;
        return results;
    }

    // Use DFS to search
    dfsSearchHelper(tree.getRoot(), title, results);

    return results;
}

// ===== SEARCH BY AUTHOR =====
vector<Komik *> SearchFilter::searchByAuthor(BST &tree, const string &author)
{
    vector<Komik *> results;

    if (author.empty())
    {
        cout << "\033[31mError: Author name cannot be empty!\033[0m" << endl;
        return results;
    }

    // Get all komiks using in-order traversal
    tree.inOrder([&results, &author, this](Komik *comic)
                 {
        if (containsIgnoreCase(comic->author, author)) {
            results.push_back(comic);
        } });

    return results;
}

// ===== SEARCH BY GENRE =====
vector<Komik *> SearchFilter::searchByGenre(BST &tree, const string &genre)
{
    vector<Komik *> results;

    if (genre.empty())
    {
        cout << "\033[31mError: Genre cannot be empty!\033[0m" << endl;
        return results;
    }

    // Get all komiks using in-order traversal
    tree.inOrder([&results, &genre, this](Komik *comic)
                 {
        if (containsIgnoreCase(comic->genre, genre)) {
            results.push_back(comic);
        } });

    return results;
}

// ===== FILTER BY GENRE (Exact Match) =====
vector<Komik *> SearchFilter::filterByGenre(BST &tree, const string &genre)
{
    vector<Komik *> results;

    tree.inOrder([&results, &genre](Komik *comic)
                 {
        // Exact match (case-insensitive)
        string lowerGenre = "";
        string lowerComicGenre = "";
        
        for (char c : genre) lowerGenre += tolower(c);
        for (char c : comic->genre) lowerComicGenre += tolower(c);
        
        if (lowerComicGenre == lowerGenre) {
            results.push_back(comic);
        } });

    return results;
}

// ===== FILTER BY AUTHOR (Exact Match) =====
vector<Komik *> SearchFilter::filterByAuthor(BST &tree, const string &author)
{
    vector<Komik *> results;

    tree.inOrder([&results, &author](Komik *comic)
                 {
        // Exact match (case-insensitive)
        string lowerAuthor = "";
        string lowerComicAuthor = "";
        
        for (char c : author) lowerAuthor += tolower(c);
        for (char c : comic->author) lowerComicAuthor += tolower(c);
        
        if (lowerComicAuthor == lowerAuthor) {
            results.push_back(comic);
        } });

    return results;
}

// ===== FILTER BY MULTIPLE (Genre AND Author) =====
vector<Komik *> SearchFilter::filterByMultiple(BST &tree, const string &genre, const string &author)
{
    vector<Komik *> results;

    tree.inOrder([&results, &genre, &author, this](Komik *comic)
                 {
        bool genreMatch = genre.empty() || containsIgnoreCase(comic->genre, genre);
        bool authorMatch = author.empty() || containsIgnoreCase(comic->author, author);
        
        if (genreMatch && authorMatch) {
            results.push_back(comic);
        } });

    return results;
}

// ===== DISPLAY RESULTS =====
void SearchFilter::displayResults(const vector<Komik *> &results, const string &message)
{
    cout << "\n\033[1;36m==============================================\033[0m" << endl;
    cout << "\033[1;36m" << message << "\033[0m" << endl;
    cout << "\033[1;36m==============================================\033[0m\n"
         << endl;

    if (results.empty())
    {
        cout << "\033[33mNo results found!\033[0m" << endl;
    }
    else
    {
        cout << "Found " << results.size() << " result(s):\n"
             << endl;

        // Print table header
        cout << left << setw(5) << "ID"
             << setw(30) << "Title"
             << setw(25) << "Author"
             << setw(20) << "Genre" << endl;
        cout << string(80, '-') << endl;

        // Print results
        for (const auto &comic : results)
        {
            cout << left << setw(5) << comic->id
                 << setw(30) << comic->title
                 << setw(25) << comic->author
                 << setw(20) << comic->genre << endl;
        }

        cout << "\n\033[32mTotal: " << results.size() << " comic(s)\033[0m" << endl;
    }

    cout << "\033[1;36m==============================================\033[0m\n"
         << endl;
}