#ifndef KOMIK_H
#define KOMIK_H

#include <string>
#include <iostream>

using namespace std;

struct Komik
{
    int id;
    string title;
    string author;
    string genre;

    Komik *left;
    Komik *right;

    Komik()
    {
        id = 0;
        title = "";
        author = "";
        genre = "";
        left = nullptr;
        right = nullptr;
    }

    Komik(int id_input, string title_input, string author_input, string genre_input)
    {
        id = id_input;
        title = title_input;
        author = author_input;
        genre = genre_input;
        left = nullptr;
        right = nullptr;
    }

    ~Komik() {}

    void display() const
    {
        cout << "ID: " << id
             << " | Title: " << title
             << " | Author: " << author
             << " | Genre: " << genre << endl;
    }

    bool operator<(const Komik &other) const
    {
        if (title != other.title)
            return title < other.title;
        if (author != other.author)
            return author < other.author;
        return genre < other.genre;
    }

    bool operator>(const Komik &other) const
    {
        if (title != other.title)
            return title > other.title;
        if (author != other.author)
            return author > other.author;
        return genre > other.genre;
    }

    bool operator==(const Komik &other) const
    {
        return title == other.title &&
               author == other.author &&
               genre == other.genre;
    }
};

#endif