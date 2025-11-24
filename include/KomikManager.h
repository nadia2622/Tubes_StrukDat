/*
 * KomikManager.h
 * File HEADER untuk Komik Manager
 * 
 * Analogi: KomikManager ini seperti "admin" yang mengelola:
 * - Sample data (10 komik contoh)
 * - Daftar genre
 * - Daftar author
 * - Daftar favorit user
 */

#ifndef KOMIK_MANAGER_H
#define KOMIK_MANAGER_H

// Include library yang dibutuhkan
#include <string>    // Untuk pakai string
#include <vector>    // Untuk pakai vector (array dinamis)
#include <map>       // Untuk pakai map (dictionary/hash map)
#include "Komik.h"   // Include Komik.h karena butuh struct Komik
#include "BST.h"     // Include BST.h karena butuh class BST

// CLASS KomikManager = class untuk mengelola data komik
class KomikManager {
private:
    // ===== PRIVATE MEMBERS =====
    // Variable-variable ini hanya bisa diakses dari dalam class
    
    int nextKomikId;  // ID untuk komik berikutnya (auto-increment)
                      // Contoh: komik pertama ID=1, kedua ID=2, dst
    
    // vector<string> = array dinamis yang bisa bertambah/berkurang
    vector<string> genres;   // Daftar genre (Action, Romance, dll)
    vector<string> authors;  // Daftar author (Masashi, Oda, dll)
    
    // map<string, vector<int>> = dictionary
    // Key = string (username)
    // Value = vector<int> (daftar ID komik favorit)
    // Contoh: userFavorites["admin"] = {1, 3, 5}
    map<string, vector<int>> userFavorites;

public:
    // ===== PUBLIC MEMBERS =====
    // Fungsi-fungsi ini bisa diakses dari luar class
    
    // ===== CONSTRUCTOR & DESTRUCTOR =====
    KomikManager();   // Constructor: dipanggil saat bikin objek baru
    ~KomikManager();  // Destructor: dipanggil saat objek dihapus
    
    // ===== KOMIK OPERATIONS =====
    
    // loadSampleData: isi tree dengan 10 komik contoh
    // Parameter: tree = reference ke BST (tidak copy, langsung pakai asli)
    void loadSampleData(BST& tree);
    
    // getNextId: ambil ID untuk komik baru
    // Return: int (ID berikutnya)
    int getNextId();
    
    // ===== GENRE OPERATIONS =====
    
    // getAllGenres: ambil semua genre
    // Return: vector berisi semua nama genre
    // const = fungsi ini tidak ubah data member
    vector<string> getAllGenres() const;
    
    // addGenre: tambah genre baru
    // Parameter: name = nama genre baru
    // Return: true jika berhasil, false jika gagal (genre sudah ada)
    bool addGenre(const string& name);
    
    // updateGenre: ubah nama genre
    // Parameter: 
    //   - oldName = nama genre lama
    //   - newName = nama genre baru
    //   - tree = reference ke BST (untuk update komik yang pakai genre ini)
    // Return: true jika berhasil, false jika gagal
    bool updateGenre(const string& oldName, const string& newName, BST& tree);
    
    // deleteGenre: hapus genre
    // Parameter: name = nama genre yang mau dihapus
    // Return: true jika berhasil, false jika gagal
    bool deleteGenre(const string& name);
    
    // genreExists: cek apakah genre ada
    // Parameter: name = nama genre
    // Return: true jika ada, false jika tidak ada
    bool genreExists(const string& name) const;
    
    // ===== AUTHOR OPERATIONS =====
    // Sama seperti genre operations, tapi untuk author
    
    vector<string> getAllAuthors() const;
    bool addAuthor(const string& name);
    bool updateAuthor(const string& oldName, const string& newName, BST& tree);
    bool deleteAuthor(const string& name);
    bool authorExists(const string& name) const;
    
    // ===== FAVORITES OPERATIONS =====
    
    // addFavorite: tambah komik ke favorit user
    // Parameter:
    //   - username = nama user
    //   - komikId = ID komik yang mau di-favorite
    void addFavorite(const string& username, int komikId);
    
    // removeFavorite: hapus komik dari favorit user
    // Parameter: sama seperti addFavorite
    // Return: true jika berhasil, false jika gagal
    bool removeFavorite(const string& username, int komikId);
    
    // getFavoritesByUser: ambil daftar favorit satu user
    // Parameter: username = nama user
    // Return: vector berisi ID-ID komik favorit user tersebut
    vector<int> getFavoritesByUser(const string& username) const;
    
    // getAllFavorites: ambil semua favorit (semua user)
    // Return: map<username, vector<ID komik>>
    map<string, vector<int>> getAllFavorites() const;
    
    // ===== UTILITY =====
    
    // reset: reset semua data (hapus semua genre, author, favorit)
    void reset();
};

#endif // KOMIK_MANAGER_H

/*
 * ===== RINGKASAN KomikManager.h =====
 * 
 * 1. CLASS KomikManager = class untuk mengelola data
 * 
 * 2. PRIVATE MEMBERS:
 *    - nextKomikId: ID auto-increment
 *    - genres: daftar genre
 *    - authors: daftar author
 *    - userFavorites: map username → daftar ID komik favorit
 * 
 * 3. PUBLIC METHODS:
 *    - loadSampleData: isi tree dengan sample data
 *    - CRUD Genre: add, update, delete, get, exists
 *    - CRUD Author: add, update, delete, get, exists
 *    - Favorites: add, remove, get by user, get all
 * 
 * 4. MENGAPA PAKAI vector?
 *    - vector = array dinamis (bisa bertambah/berkurang)
 *    - Lebih flexible dari array biasa
 * 
 * 5. MENGAPA PAKAI map?
 *    - map = dictionary (key-value pairs)
 *    - Cocok untuk favorit: key=username, value=list komik
 *    - Cari data cepat: O(log n)
 * 
 * 6. MENGAPA PAKAI & (reference)?
 *    - & = reference (tidak copy data, pakai yang asli)
 *    - Hemat memori
 *    - Perubahan langsung ke data asli
 */