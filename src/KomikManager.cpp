/*
 * KomikManager.cpp
 * File IMPLEMENTASI Komik Manager
 *
 * Di file ini kita tulis detail dari semua fungsi yang ada di KomikManager.h
 */

#include "../include/KomikManager.h" // Include header KomikManager
#include <iostream>                  // Untuk pakai cout, cerr
#include <algorithm>                 // Untuk pakai find() (fungsi cari)

using namespace std; // Biar tidak perlu tulis std:: terus

// ===== CONSTRUCTOR =====
// Dipanggil saat: KomikManager manager;
KomikManager::KomikManager()
{
    // : nextKomikId(1) = initialization list
    // Set nextKomikId = 1 (ID pertama dimulai dari 1)
    nextKomikId = 1;

    // ===== INISIALISASI GENRES =====
    // genres = {...} adalah "initializer list" untuk isi vector
    // Ini cara cepat untuk isi vector dengan banyak data sekaligus
    genres = {
        "Action",  // Index 0
        "Romance", // Index 1
        "Comedy",  // Index 2
        "Horror",  // Index 3
        "Fantasy", // Index 4
        "Mystery", // Index 5
        "Drama",   // Index 6
        "Sci-Fi"   // Index 7
    };
    // Sekarang genres punya 8 element

    // ===== INISIALISASI AUTHORS =====
    authors = {
        "Masashi Kishimoto", // Pengarang Naruto
        "Eiichiro Oda",      // Pengarang One Piece
        "Akira Toriyama",    // Pengarang Dragon Ball
        "Hajime Isayama",    // Pengarang Attack on Titan
        "Kentaro Miura",     // Pengarang Berserk
        "Tsugumi Ohba",      // Pengarang Death Note
        "Yoshihiro Togashi", // Pengarang Hunter x Hunter
        "Tite Kubo"          // Pengarang Bleach
    };

    // ===== INISIALISASI SAMPLE FAVORITES =====
    // userFavorites adalah map<string, vector<int>>
    // Syntax: userFavorites[key] = {value1, value2, ...}

    userFavorites["user1"] = {1, 2, 3};
    // user1 suka komik ID 1, 2, dan 3

    userFavorites["user2"] = {2, 4};
    // user2 suka komik ID 2 dan 4

    userFavorites["admin"] = {1, 3, 5};
    // admin suka komik ID 1, 3, dan 5
}

// ===== DESTRUCTOR =====
KomikManager::~KomikManager()
{
    // Destructor kosong karena:
    // - vector dan map otomatis dibersihkan oleh C++
    // - Tidak ada pointer yang perlu di-delete manual
    // - Tidak ada file yang perlu ditutup
}

// ===== LOAD SAMPLE DATA =====
// Fungsi ini mengisi tree dengan 10 komik contoh
void KomikManager::loadSampleData(BST &tree)
{
    // BST& tree = reference ke tree (tidak copy, pakai yang asli)

    cout << "Loading sample data..." << endl;

    // Clear existing tree (hapus komik yang sudah ada)
    tree.clear();

    // ===== DEFINISI STRUCT LOKAL =====
    // Struct ini hanya ada di dalam fungsi ini (local scope)
    struct KomikData
    {
        string title;
        string author;
        string genre;
    };
    // Struct ini untuk sementara simpan data komik sebelum dimasukkan ke tree

    // ===== BIKIN VECTOR SAMPLE KOMIK =====
    // vector<KomikData> = vector yang isinya KomikData
    vector<KomikData> sampleKomiks = {
        // Syntax: {title, author, genre}
        {"Naruto", "Masashi Kishimoto", "Action"},
        {"One Piece", "Eiichiro Oda", "Action"},
        {"Dragon Ball", "Akira Toriyama", "Action"},
        {"Attack on Titan", "Hajime Isayama", "Action"},
        {"Berserk", "Kentaro Miura", "Fantasy"},
        {"Death Note", "Tsugumi Ohba", "Mystery"},
        {"Hunter x Hunter", "Yoshihiro Togashi", "Action"},
        {"Bleach", "Tite Kubo", "Action"},
        {"Fullmetal Alchemist", "Hiromu Arakawa", "Fantasy"},
        {"Tokyo Ghoul", "Sui Ishida", "Horror"}};
    // Total: 10 komik sample

    // ===== INSERT SAMPLE DATA KE TREE =====
    int id = 1; // ID dimulai dari 1

    // for (const auto& comic : sampleKomiks) = range-based for loop
    // Artinya: untuk setiap comic di dalam sampleKomiks
    // const auto& = otomatis detect tipe (KomikData), dan pakai reference (tidak copy)
    for (const auto &comic : sampleKomiks)
    {
        // Bikin node Komik baru dengan new (alokasi di heap/memory dinamis)
        Komik *newKomik = new Komik(id++, comic.title, comic.author, comic.genre);
        // id++ = pakai id sekarang, terus increment (tambah 1)
        // Jadi ID: 1, 2, 3, 4, ..., 10

        // Insert komik ke tree
        tree.insert(newKomik);
    }

    // Update nextKomikId untuk ID komik selanjutnya
    nextKomikId = id; // Sekarang id = 11

    cout << "Loaded " << (id - 1) << " sample comics!" << endl;
    // id - 1 karena id sudah di-increment jadi 11, tapi komik cuma 10
}

// ===== GET NEXT ID =====
// Fungsi untuk ambil ID berikutnya (auto-increment)
int KomikManager::getNextId()
{
    // return nextKomikId++; artinya:
    // 1. Return nilai nextKomikId sekarang
    // 2. Lalu increment nextKomikId (tambah 1)
    // Contoh: jika nextKomikId = 11, return 11, terus nextKomikId jadi 12
    return nextKomikId++;
}

// ===== GENRE OPERATIONS =====

// getAllGenres: return semua genre
vector<string> KomikManager::getAllGenres() const
{
    // const = fungsi ini tidak ubah data member
    return genres; // Return copy dari vector genres
}

// addGenre: tambah genre baru
bool KomikManager::addGenre(const string &name)
{
    // const string& name = reference ke string (tidak copy)

    // Cek apakah genre sudah ada
    if (genreExists(name))
    {
        // cerr = print ke error stream (warna merah di terminal)
        cerr << "Genre '" << name << "' already exists!" << endl;
        return false; // Gagal tambah genre
    }

    // Tambah genre ke vector
    // push_back() = tambah element di akhir vector
    genres.push_back(name);

    cout << "Genre '" << name << "' added successfully!" << endl;
    return true; // Berhasil
}

// updateGenre: ubah nama genre
bool KomikManager::updateGenre(const string &oldName, const string &newName, BST &tree)
{
    // ===== CEK APAKAH GENRE LAMA ADA =====
    // find() = fungsi dari <algorithm> untuk cari element di vector
    // Syntax: find(begin, end, value)
    // Return: iterator (pointer) ke element yang ditemukan, atau end() jika tidak ketemu
    auto it = find(genres.begin(), genres.end(), oldName);
    // auto = otomatis detect tipe (vector<string>::iterator)
    // it = iterator (seperti pointer ke element di vector)

    if (it == genres.end())
    {
        // end() = posisi setelah element terakhir (artinya: tidak ketemu)
        cerr << "Genre '" << oldName << "' not found!" << endl;
        return false;
    }

    // ===== CEK APAKAH NAMA BARU SUDAH ADA =====
    if (oldName != newName && genreExists(newName))
    {
        // Jika nama berubah DAN nama baru sudah ada = duplikat!
        cerr << "Genre '" << newName << "' already exists!" << endl;
        return false;
    }

    // ===== UPDATE GENRE DI LIST =====
    *it = newName;
    // *it = dereferencing iterator, ubah value di posisi iterator

    // ===== UPDATE SEMUA KOMIK YANG PAKAI GENRE INI =====
    // Ambil semua komik dari tree
    vector<Komik *> allKomiks = tree.getAllKomiks();

    // Loop setiap komik
    for (Komik *comic : allKomiks)
    {
        // Jika genre komik = genre lama, ubah ke genre baru
        if (comic->genre == oldName)
        {
            comic->genre = newName;
        }
    }

    cout << "Genre updated successfully!" << endl;
    return true;
}

// deleteGenre: hapus genre
bool KomikManager::deleteGenre(const string &name)
{
    // Cari genre
    auto it = find(genres.begin(), genres.end(), name);

    if (it == genres.end())
    {
        cerr << "Genre '" << name << "' not found!" << endl;
        return false;
    }

    // Hapus genre dari vector
    // erase() = hapus element di posisi iterator
    genres.erase(it);

    cout << "Genre '" << name << "' deleted successfully!" << endl;
    return true;
}

// genreExists: cek apakah genre ada
bool KomikManager::genreExists(const string &name) const
{
    // find() return iterator
    // Jika iterator != end(), berarti ketemu (ada)
    return find(genres.begin(), genres.end(), name) != genres.end();
}

// ===== AUTHOR OPERATIONS =====
// Fungsi-fungsi author sama persis dengan genre operations
// Hanya beda variable: genres → authors

vector<string> KomikManager::getAllAuthors() const
{
    return authors;
}

bool KomikManager::addAuthor(const string &name)
{
    if (authorExists(name))
    {
        cerr << "Author '" << name << "' already exists!" << endl;
        return false;
    }
    authors.push_back(name);
    cout << "Author '" << name << "' added successfully!" << endl;
    return true;
}

bool KomikManager::updateAuthor(const string &oldName, const string &newName, BST &tree)
{
    auto it = find(authors.begin(), authors.end(), oldName);
    if (it == authors.end())
    {
        cerr << "Author '" << oldName << "' not found!" << endl;
        return false;
    }

    if (oldName != newName && authorExists(newName))
    {
        cerr << "Author '" << newName << "' already exists!" << endl;
        return false;
    }

    *it = newName;

    // Update semua komik yang pakai author ini
    vector<Komik *> allKomiks = tree.getAllKomiks();
    for (Komik *comic : allKomiks)
    {
        if (comic->author == oldName)
        {
            comic->author = newName;
        }
    }

    cout << "Author updated successfully!" << endl;
    return true;
}

bool KomikManager::deleteAuthor(const string &name)
{
    auto it = find(authors.begin(), authors.end(), name);
    if (it == authors.end())
    {
        cerr << "Author '" << name << "' not found!" << endl;
        return false;
    }
    authors.erase(it);
    cout << "Author '" << name << "' deleted successfully!" << endl;
    return true;
}

bool KomikManager::authorExists(const string &name) const
{
    return find(authors.begin(), authors.end(), name) != authors.end();
}

// ===== FAVORITES OPERATIONS =====

// addFavorite: tambah komik ke favorit user
void KomikManager::addFavorite(const string &username, int comicId)
{
    // ===== AMBIL REFERENCE KE VECTOR FAVORIT USER =====
    // userFavorites[username] = akses/bikin entry untuk username
    // Jika username belum ada, otomatis dibikin entry baru dengan vector kosong
    // & = reference (bukan copy)
    auto &favs = userFavorites[username];
    // favs sekarang adalah reference ke vector<int> milik user tersebut

    // ===== CEK APAKAH SUDAH DI-FAVORITE =====
    // find() = cari comicId di vector favs
    if (find(favs.begin(), favs.end(), comicId) != favs.end())
    {
        // Jika ketemu (sudah ada), print pesan dan keluar
        cout << "Komik already in favorites!" << endl;
        return;
    }

    // ===== TAMBAH KE FAVORIT =====
    favs.push_back(comicId); // Tambah comicId ke vector
    cout << "Komik added to favorites!" << endl;
}

// removeFavorite: hapus komik dari favorit user
bool KomikManager::removeFavorite(const string &username, int comicId)
{
    // ===== CEK APAKAH USER ADA =====
    // find() untuk map = cari key
    if (userFavorites.find(username) == userFavorites.end())
    {
        // User tidak punya favorit
        return false;
    }

    // ===== AMBIL REFERENCE KE VECTOR FAVORIT =====
    auto &favs = userFavorites[username];

    // ===== CARI COMIC DI FAVORIT =====
    auto it = find(favs.begin(), favs.end(), comicId);

    if (it == favs.end())
    {
        // Comic tidak ada di favorit
        return false;
    }

    // ===== HAPUS DARI FAVORIT =====
    favs.erase(it);
    cout << "Komik removed from favorites!" << endl;
    return true;
}

// getFavoritesByUser: ambil favorit satu user
vector<int> KomikManager::getFavoritesByUser(const string &username) const
{
    // ===== CEK APAKAH USER ADA =====
    if (userFavorites.find(username) == userFavorites.end())
    {
        // User tidak ada, return vector kosong
        return vector<int>(); // Vector kosong
    }

    // ===== RETURN FAVORIT USER =====
    // at() = akses element map dengan key
    // Bedanya dengan []: at() throw exception jika key tidak ada
    return userFavorites.at(username);
}

// getAllFavorites: return semua favorit
map<string, vector<int>> KomikManager::getAllFavorites() const
{
    return userFavorites; // Return copy dari map
}

// ===== UTILITY =====

// reset: reset semua data
void KomikManager::reset()
{
    nextKomikId = 1;       // Reset ID ke 1
    genres.clear();        // Hapus semua genre
    authors.clear();       // Hapus semua author
    userFavorites.clear(); // Hapus semua favorit
    cout << "Manager reset!" << endl;
}

/*
 * ===== RINGKASAN KomikManager.cpp =====
 *
 * 1. CONSTRUCTOR:
 *    - Inisialisasi nextKomikId = 1
 *    - Isi genres dengan 8 genre default
 *    - Isi authors dengan 8 author default
 *    - Isi sample favorites untuk 3 user
 *
 * 2. LOAD SAMPLE DATA:
 *    - Clear tree dulu
 *    - Bikin 10 komik sample
 *    - Insert satu-satu ke tree
 *    - Update nextKomikId
 *
 * 3. GENRE OPERATIONS:
 *    - getAllGenres: return vector genres
 *    - addGenre: pakai push_back()
 *    - updateGenre: pakai find() + erase() + push_back()
 *    - deleteGenre: pakai find() + erase()
 *    - genreExists: pakai find()
 *
 * 4. AUTHOR OPERATIONS:
 *    - Sama seperti genre operations
 *
 * 5. FAVORITES OPERATIONS:
 *    - addFavorite: akses map, push_back ke vector
 *    - removeFavorite: akses map, find + erase
 *    - getFavorites: akses map dengan at()
 *
 * 6. KONSEP PENTING:
 *    - vector: array dinamis, pakai push_back() untuk tambah
 *    - map: dictionary, pakai [] atau at() untuk akses
 *    - find(): cari element, return iterator
 *    - erase(): hapus element di posisi iterator
 *    - auto: otomatis detect tipe variable
 *    - &: reference (tidak copy, pakai asli)
 */