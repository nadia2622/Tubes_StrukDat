/*
 * Komik.h
 * File ini berisi STRUKTUR NODE untuk Binary Search Tree
 *
 * Analogi: Bayangkan setiap komik adalah sebuah "kotak" yang berisi:
 * - Data komik (id, title, author, genre)
 * - 2 tali pengikat ke kotak lain (left dan right pointer)
 */

#ifndef KOMIK_H
#define KOMIK_H

// Include library yang dibutuhkan
#include <string>   // Untuk pakai tipe data string (title, author, genre)
#include <iostream> // Untuk pakai cout (print ke layar)

using namespace  std;

// STRUCT = struktur data, kayak blueprint/cetakan untuk bikin node
struct Komik
{
    // ===== DATA KOMIK (isi dari node) =====
    int id;             // Nomor ID komik (unique, tidak ada yang sama)
    string title;  // Judul komik (contoh: "Naruto")
    string author; // Nama pengarang (contoh: "Masashi Kishimoto")
    string genre;  // Genre komik (contoh: "Action")

    // ===== POINTER UNTUK BST (tali pengikat ke node lain) =====
    // Pointer = alamat memori yang menunjuk ke node lain
    Komik *left;  // Pointer ke anak kiri (komik yang judulnya lebih kecil)
    Komik *right; // Pointer ke anak kanan (komik yang judulnya lebih besar)

    // ===== CONSTRUCTOR = fungsi yang dijalankan saat bikin node baru =====

    // Constructor 1: Bikin node kosong
    // Dipanggil ketika: Komik komik1;
    Komik()
    {
        id = 0;          // ID di-set 0
        title = "";      // Title kosong
        author = "";     // Author kosong
        genre = "";      // Genre kosong
        left = nullptr;  // Pointer kiri dikosongkan (nullptr = tidak menunjuk kemana-mana)
        right = nullptr; // Pointer kanan dikosongkan
    }

    // Constructor 2: Bikin node dengan data
    // Dipanggil ketika: Komik komik1(1, "Naruto", "Masashi", "Action");
    Komik(int id_input, string title_input, string author_input, string genre_input)
    {
        id = id_input;         // Masukkan ID dari parameter
        title = title_input;   // Masukkan title dari parameter
        author = author_input; // Masukkan author dari parameter
        genre = genre_input;   // Masukkan genre dari parameter
        left = nullptr;        // Pointer kiri kosongkan dulu
        right = nullptr;       // Pointer kanan kosongkan dulu
    }

    // ===== DESTRUCTOR = fungsi yang dijalankan saat node dihapus =====
    // Destructor dipanggil otomatis ketika: delete komik1;
    ~Komik()
    {
        // Kosong karena tidak ada yang perlu dibersihkan
        // (string dan int otomatis dibersihkan oleh C++)
    }

    // ===== METHOD DISPLAY = fungsi untuk print info komik =====
    void display() const
    {
        // const = fungsi ini tidak mengubah data komik
        // Contoh output: ID: 1 | Title: Naruto | Author: Masashi | Genre: Action
        cout << "ID: " << id
                  << " | Title: " << title
                  << " | Author: " << author
                  << " | Genre: " << genre << endl;
    }

    // ===== OPERATOR PERBANDINGAN (untuk sorting di BST) =====

    // Operator < (kurang dari)
    // Fungsi: Membandingkan 2 komik, mana yang lebih kecil
    // Digunakan saat: if (komik1 < komik2)
    bool operator<(const Komik &other) const
    {
        // const Komik& other = referensi ke komik lain (tidak copy, langsung pakai yang asli)
        // const = tidak boleh mengubah data

        // CARA PERBANDINGAN (hierarki):
        // 1. Bandingkan TITLE dulu
        if (title != other.title)
        {
            // Jika title berbeda, return hasil perbandingan title
            // Contoh: "Attack on Titan" < "Naruto" = true (A lebih kecil dari N)
            return title < other.title;
        }

        // 2. Jika title SAMA, bandingkan AUTHOR
        if (author != other.author)
        {
            // Contoh: Jika 2 komik sama-sama "Naruto",
            // bandingkan author: "Masashi" vs "Oda"
            return author < other.author;
        }

        // 3. Jika title DAN author SAMA, bandingkan GENRE
        // Contoh: Jika title dan author sama,
        // bandingkan genre: "Action" vs "Comedy"
        return genre < other.genre;
    }

    // Operator > (lebih dari)
    // Fungsi: Kebalikan dari operator <
    // Digunakan saat: if (komik1 > komik2)
    bool operator>(const Komik &other) const
    {
        // Logikanya sama persis dengan operator <, tapi kebalikan
        if (title != other.title)
        {
            return title > other.title;
        }
        if (author != other.author)
        {
            return author > other.author;
        }
        return genre > other.genre;
    }

    // Operator == (sama dengan)
    // Fungsi: Cek apakah 2 komik identik (title, author, genre sama semua)
    // Digunakan saat: if (komik1 == komik2)
    bool operator==(const Komik &other) const
    {
        // Harus SEMUA field sama
        // Operator && = AND (semua harus true)
        return title == other.title &&   // Title sama DAN
               author == other.author && // Author sama DAN
               genre == other.genre;     // Genre sama
    }
};

#endif // KOMIK_H

/*
 * ===== RINGKASAN KOMIK.H =====
 *
 * 1. STRUCT Komik = cetakan untuk bikin node BST
 *
 * 2. ISI NODE:
 *    - Data: id, title, author, genre
 *    - Pointer: left, right (untuk hubungkan ke node lain)
 *
 * 3. CONSTRUCTOR:
 *    - Constructor kosong: bikin node kosong
 *    - Constructor dengan parameter: bikin node dengan data
 *
 * 4. METHOD:
 *    - display(): print info komik
 *
 * 5. OPERATOR:
 *    - operator<: bandingkan mana yang lebih kecil
 *    - operator>: bandingkan mana yang lebih besar
 *    - operator==: cek apakah sama
 *
 * Operator ini dipakai di BST untuk sorting otomatis!
 */