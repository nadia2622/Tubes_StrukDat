/*
 * BST.h
 * File HEADER untuk Binary Search Tree
 *
 * Analogi: Ini adalah "daftar isi" dari BST
 * Di file .h kita tulis DEKLARASI (rencana) fungsi-fungsinya
 * Di file .cpp nanti kita tulis IMPLEMENTASI (isi detail) fungsinya
 */

#ifndef BST_H
#define BST_H

#include "Komik.h"    // Include Komik.h karena kita butuh struct Komik
#include <functional> // Include functional untuk pakai std::function
#include <vector>     // Include vector untuk menyimpan banyak data

using namespace std; // Biar tidak perlu tulis std:: terus-menerus

// CLASS BST = blueprint untuk bikin pohon BST
class BST
{
private:
    // ===== PRIVATE = hanya bisa diakses dari dalam class BST =====

    Komik *root; // root = node paling atas di tree
                 // Semua tree dimulai dari root
                 // Pointer karena bisa kosong (nullptr) atau menunjuk ke node

    // ===== HELPER FUNCTIONS (fungsi pembantu) =====
    // Fungsi-fungsi ini PRIVATE karena hanya dipakai internal
    // User tidak perlu tahu detail implementasinya

    // insertHelper: fungsi rekursif untuk insert node
    // Parameter: node = node saat ini, newKomik = komik baru yang mau dimasukkan
    // Return: pointer ke node (hasil setelah insert)
    Komik *insertHelper(Komik *node, Komik *newKomik);

    // searchHelper: fungsi rekursif untuk cari komik by title
    // Parameter: node = node saat ini, title = judul yang dicari
    // Return: pointer ke komik yang ditemukan (atau nullptr jika tidak ada)
    Komik *searchHelper(Komik *node, const string &title) const;

    // deleteHelper: fungsi rekursif untuk hapus node
    // Parameter: node = node saat ini, title = judul yang mau dihapus
    // Return: pointer ke node (hasil setelah delete)
    Komik *deleteHelper(Komik *node, const string &title);

    // findMin: cari node terkecil di subtree
    // Dipakai untuk kasus delete node dengan 2 anak
    // Parameter: node = node awal pencarian
    // Return: pointer ke node terkecil
    Komik *findMin(Komik *node) const;

    // ===== TRAVERSAL HELPERS =====
    // function<void(Komik*)> = tipe data fungsi yang:
    //   - Menerima parameter Komik*
    //   - Tidak return apa-apa (void)
    // Dipakai untuk callback (fungsi yang dijalankan untuk setiap node)

    void preOrderHelper(Komik *node, function<void(Komik *)> callback) const;
    void inOrderHelper(Komik *node, function<void(Komik *)> callback) const;
    void postOrderHelper(Komik *node, function<void(Komik *)> callback) const;

    // destroyTree: hapus semua node di tree (untuk cleanup memory)
    // Parameter: node = node awal yang mau dihapus
    void destroyTree(Komik *node);

    // countHelper: hitung jumlah node di tree secara rekursif
    // Parameter: node = node saat ini
    // Return: jumlah node (int)
    int countHelper(Komik *node) const;

public:
    // ===== PUBLIC = bisa diakses dari luar class BST =====
    // Fungsi-fungsi ini adalah INTERFACE yang dipakai user

    // ===== CONSTRUCTOR & DESTRUCTOR =====

    // Constructor: dipanggil saat bikin objek BST baru
    // Contoh: BST tree;
    BST();

    // Destructor: dipanggil saat objek BST dihapus
    // Fungsi: membersihkan semua node (free memory)
    ~BST();

    // ===== OPERASI UTAMA BST =====

    // insert: masukkan komik baru ke tree
    // Parameter: newKomik = pointer ke komik baru
    void insert(Komik *newKomik);

    // search: cari komik berdasarkan title
    // Parameter: title = judul yang dicari
    // Return: pointer ke komik (atau nullptr jika tidak ada)
    Komik *search(const string &title) const;

    // remove: hapus komik dari tree berdasarkan title
    // Parameter: title = judul yang mau dihapus
    // Return: true jika berhasil, false jika gagal
    bool remove(const string &title);

    // update: update data komik
    // Parameter: oldTitle = judul lama, updatedKomik = data baru
    // Return: true jika berhasil, false jika gagal
    bool update(const string &oldTitle, Komik *updatedKomik);

    // ===== TRAVERSAL METHODS =====
    // Traversal = mengunjungi semua node dengan urutan tertentu

    // preOrder: Root → Left → Right
    // Parameter: callback = fungsi yang dijalankan untuk setiap node
    void preOrder(function<void(Komik *)> callback) const;

    // inOrder: Left → Root → Right (hasil terurut!)
    // Parameter: callback = fungsi yang dijalankan untuk setiap node
    void inOrder(function<void(Komik *)> callback) const;

    // postOrder: Left → Right → Root
    // Parameter: callback = fungsi yang dijalankan untuk setiap node
    void postOrder(function<void(Komik *)> callback) const;

    // ===== UTILITY METHODS =====

    // isEmpty: cek apakah tree kosong
    // Return: true jika kosong, false jika ada isi
    bool isEmpty() const;

    // count: hitung total komik di tree
    // Return: jumlah komik (int)
    int count() const;

    // clear: hapus semua komik di tree
    void clear();

    // getAllKomiks: ambil semua komik dalam bentuk vector
    // Return: vector berisi pointer ke semua komik
    vector<Komik *> getAllKomiks() const;

    // getRoot: ambil pointer root (untuk testing/debugging)
    // Return: pointer ke root
    Komik *getRoot() const
    {
        return root; // Inline function: langsung return root
    }
};

#endif // BST_H

/*
 * ===== RINGKASAN BST.H =====
 *
 * 1. CLASS BST = template untuk bikin pohon binary search tree
 *
 * 2. PRIVATE MEMBERS:
 *    - root: node paling atas
 *    - Helper functions: fungsi internal untuk rekursi
 *
 * 3. PUBLIC MEMBERS:
 *    - Constructor & Destructor
 *    - Insert, Search, Remove, Update (CRUD)
 *    - PreOrder, InOrder, PostOrder (Traversal)
 *    - isEmpty, count, clear, getAll (Utility)
 *
 * 4. MENGAPA ADA HELPER?
 *    - User pakai fungsi public (contoh: insert)
 *    - Fungsi public panggil fungsi helper (insertHelper) untuk rekursi
 *    - Ini pattern umum dalam implementasi tree
 *
 * 5. MENGAPA PAKAI const?
 *    - const di parameter = tidak boleh ubah data
 *    - const di akhir fungsi = fungsi tidak ubah member variable
 *    - Ini untuk keamanan dan mencegah bug
 */