/* Tujuan header ini: menyediakan fungsi-fungsi pencarian ke BST (komik), 
 * berdasarkan title, genre, dan author, semuanya memakai DFS (inOrder traversal).
 *
 * SearchFilter.h
 * Fitur pencarian komik berdasarkan:
 * - Title (DFS)
 * - Genre (DFS)
 * - Author (DFS)
 *
 * Semua pencarian menggunakan inOrder DFS agar hasil terurut.
 * Pencarian bersifat partial match dan case-insensitive.
 */

#ifndef SEARCH_FILTER_H
#define SEARCH_FILTER_H

#include <string>
#include <vector>
#include <algorithm>    // for transform (lowercase)
#include "Komik.h"
#include "BST.h"

using namespace std;

class SearchFilter {
public:
    // ===== SEARCH BY TITLE =====
    // Mencari komik berdasarkan title (mengandung keyword)
    /*
    Fungsi ini mencari komik berdasarkan judul.
    Contoh:
    keyword = "nar"
    akan cocok dengan "Naruto", "Narnia", "Naru".
    DFS akan mengecek semua node komik dalam tree, kemudian memasukkan ke vector kalau:
    */
    vector<Komik*> searchByTitle(const BST& tree, const string& keyword) const;

    // ===== SEARCH BY GENRE =====
    // Mencari komik berdasarkan genre (mengandung keyword)
    /*
    Mencari komik berdasarkan genre.
    Contoh:
    keyword = "act" → cocok dengan "Action"
    keyword = "rom" → cocok dengan "Romance"
    Logikanya sama dengan title, tapi pengecekan dilakukan ke: komik->genre
    */
    vector<Komik*> searchByGenre(const BST& tree, const string& keyword) const;

    // ===== SEARCH BY AUTHOR =====
    // Mencari komik berdasarkan author (mengandung keyword)
    /*
    Mencari komik berdasarkan nama author.
    Contoh:
    keyword = "oda" → cocok dengan "Eiichiro Oda"
    keyword = "masas" → cocok dengan "Masashi Kishimoto"
    Ini mengecek:
    komik->author
    */
    vector<Komik*> searchByAuthor(const BST& tree, const string& keyword) const;

private:
    // helper function: ubah string menjadi lowercase
    /*
    Fungsi ini ditambahkan agar pencarian:
        - tidak peduli huruf besar/kecil (case-insensitive)
        - aman dipakai oleh tiga fungsi search di atas
    Cara kerjanya (nanti di .cpp):
        - meng-copy string
        - mengubah semua hurufnya menjadi lowercase
        - return string lowercase tersebut
    */
    string toLower(const string& str) const;
};

#endif // SEARCH_FILTER_H
