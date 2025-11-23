/*
 * BST.cpp
 * File IMPLEMENTASI Binary Search Tree
 *
 * Di file ini kita tulis DETAIL dari semua fungsi yang sudah dideklarasikan di BST.h
 */

#include "../include/BST.h" // Include BST.h untuk akses deklarasi class BST
#include <iostream>         // Include iostream untuk pakai cout

// ===== CONSTRUCTOR =====
// Dipanggil saat: BST tree;
BST::BST()
{
    // BST::BST() artinya: fungsi BST() milik class BST
    // : root(nullptr) = initialization list, set root = nullptr
    root = nullptr; // Set root kosong (tree masih kosong)
}

// ===== DESTRUCTOR =====
// Dipanggil saat: objek BST dihapus (keluar dari scope atau delete)
BST::~BST()
{
    // Destructor harus membersihkan SEMUA node untuk hindari memory leak
    // Memory leak = memori yang sudah tidak dipakai tapi tidak di-free
    destroyTree(root); // Panggil fungsi destroyTree untuk hapus semua node
}

// ===== DESTROY TREE (rekursif) =====
// Fungsi ini REKURSIF = memanggil dirinya sendiri
void BST::destroyTree(Komik *node)
{
    // Base case: jika node kosong, tidak ada yang dihapus
    if (node == nullptr)
    {
        return; // Keluar dari fungsi
    }

    // Recursive case: hapus left subtree dulu
    destroyTree(node->left);

    // Hapus right subtree
    destroyTree(node->right);

    // Baru hapus node saat ini
    // Urutan: kiri → kanan → tengah (post-order)
    // Kenapa? Karena kita tidak bisa hapus parent sebelum hapus children
    delete node; // Free memory yang dipakai node
}

// ===== INSERT (public interface) =====
// Fungsi yang dipanggil user untuk insert komik baru
void BST::insert(Komik *newKomik)
{
    // User panggil: tree.insert(komikBaru);
    // Fungsi ini hanya "gerbang" yang panggil insertHelper

    root = insertHelper(root, newKomik);
    // Kenapa root = ... ?
    // Karena insertHelper return node baru (bisa berubah)
}

// ===== INSERT HELPER (rekursif) =====
// Fungsi rekursif untuk insert komik ke posisi yang tepat
Komik *BST::insertHelper(Komik *node, Komik *newKomik)
{
    // Parameter:
    // - node: node saat ini yang sedang diperiksa
    // - newKomik: komik baru yang mau dimasukkan

    // BASE CASE: jika node kosong, berarti ini posisi yang tepat!
    if (node == nullptr)
    {
        return newKomik; // Return komik baru sebagai node di posisi ini
    }

    // RECURSIVE CASE: bandingkan newKomik dengan node saat ini

    // Pakai operator< yang sudah kita buat di Komik.h
    if (*newKomik < *node)
    {
        // Jika newKomik lebih kecil, masuk ke LEFT subtree
        // *newKomik = dereferencing pointer, ambil isi dari pointer

        node->left = insertHelper(node->left, newKomik);
        // Panggil insertHelper secara rekursif ke kiri
        // Hasil rekursi di-assign ke node->left
    }
    else if (*newKomik > *node)
    {
        // Jika newKomik lebih besar, masuk ke RIGHT subtree
        node->right = insertHelper(node->right, newKomik);
    }
    else
    {
        // Jika SAMA (title, author, genre sama semua)
        // UPDATE data yang ada, jangan insert duplikat

        node->id = newKomik->id;
        node->author = newKomik->author;
        node->genre = newKomik->genre;

        delete newKomik; // Hapus komik baru karena tidak jadi dipakai
    }

    return node; // Return node (bisa berubah atau tetap)
}

// ===== SEARCH (public interface) =====
// Fungsi untuk cari komik berdasarkan title
Komik *BST::search(const string &title) const
{
    // const string& title = referensi ke string (tidak copy, hemat memori)
    // const di akhir = fungsi ini tidak mengubah data tree

    return searchHelper(root, title);
    // Panggil searchHelper mulai dari root
}

// ===== SEARCH HELPER (rekursif) =====
Komik *BST::searchHelper(Komik *node, const string &title) const
{
    // BASE CASE 1: node kosong, komik tidak ditemukan
    if (node == nullptr)
    {
        return nullptr; // Return null = tidak ketemu
    }

    // BASE CASE 2: ketemu! title sama dengan node->title
    if (node->title == title)
    {
        return node; // Return pointer ke komik yang ditemukan
    }

    // RECURSIVE CASE: cari di subtree yang sesuai

    if (title < node->title)
    {
        // Jika title lebih kecil, cari di LEFT
        return searchHelper(node->left, title);
    }
    else
    {
        // Jika title lebih besar, cari di RIGHT
        return searchHelper(node->right, title);
    }
}

// ===== REMOVE (public interface) =====
// Fungsi untuk hapus komik dari tree
bool BST::remove(const string &title)
{
    // Cek dulu apakah komik ada
    if (search(title) == nullptr)
    {
        return false; // Komik tidak ada, gagal hapus
    }

    // Panggil deleteHelper untuk hapus
    root = deleteHelper(root, title);
    return true; // Berhasil hapus
}

// ===== DELETE HELPER (rekursif) =====
// Fungsi rekursif untuk hapus node
// Ini fungsi PALING KOMPLEKS karena ada 3 kasus!
Komik *BST::deleteHelper(Komik *node, const string &title)
{
    // BASE CASE: node kosong
    if (node == nullptr)
    {
        return nullptr;
    }

    // RECURSIVE CASE: cari node yang mau dihapus
    if (title < node->title)
    {
        // Cari di left subtree
        node->left = deleteHelper(node->left, title);
    }
    else if (title > node->title)
    {
        // Cari di right subtree
        node->right = deleteHelper(node->right, title);
    }
    else
    {
        // KETEMU! Node ini yang mau dihapus
        // Ada 3 KASUS:

        // KASUS 1: Node LEAF (tidak punya anak)
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;    // Hapus node
            return nullptr; // Return null (posisi ini jadi kosong)
        }

        // KASUS 2A: Node punya 1 anak (anak KANAN saja)
        else if (node->left == nullptr)
        {
            Komik *temp = node->right; // Simpan anak kanan
            delete node;               // Hapus node
            return temp;               // Return anak kanan (naik ke posisi parent)
        }

        // KASUS 2B: Node punya 1 anak (anak KIRI saja)
        else if (node->right == nullptr)
        {
            Komik *temp = node->left; // Simpan anak kiri
            delete node;              // Hapus node
            return temp;              // Return anak kiri (naik ke posisi parent)
        }

        // KASUS 3: Node punya 2 anak
        // Ini kasus paling tricky!
        else
        {
            // Strategi: Cari SUCCESSOR
            // Successor = node terkecil di right subtree
            // Kenapa? Karena successor lebih besar dari left subtree
            // tapi lebih kecil dari sisa right subtree

            Komik *successor = findMin(node->right);

            // COPY data successor ke node saat ini
            node->id = successor->id;
            node->title = successor->title;
            node->author = successor->author;
            node->genre = successor->genre;

            // HAPUS successor (karena datanya sudah di-copy)
            node->right = deleteHelper(node->right, successor->title);
        }
    }

    return node; // Return node (bisa berubah atau tetap)
}

// ===== FIND MIN =====
// Cari node terkecil di subtree (paling kiri)
Komik *BST::findMin(Komik *node) const
{
    // Node terkecil = node yang paling kiri
    // Jalan terus ke kiri sampai tidak bisa lagi

    while (node->left != nullptr)
    {
        node = node->left; // Pindah ke kiri
    }

    return node; // Node ini yang terkecil
}

// ===== UPDATE =====
// Update data komik
bool BST::update(const string &oldTitle, Komik *updatedKomik)
{
    // Cari komik yang mau diupdate
    Komik *comic = search(oldTitle);

    if (comic == nullptr)
    {
        // Komik tidak ditemukan
        delete updatedKomik; // Hapus komik baru (tidak jadi dipakai)
        return false;
    }

    // Ada 2 kasus:

    // KASUS 1: Title BERUBAH
    if (oldTitle != updatedKomik->title)
    {
        // Jika title berubah, posisi node di tree juga berubah!
        // Solusi: hapus node lama, insert node baru

        remove(oldTitle);     // Hapus node lama
        insert(updatedKomik); // Insert node baru
    }
    // KASUS 2: Title TIDAK BERUBAH
    else
    {
        // Jika title tidak berubah, cukup update data saja
        comic->id = updatedKomik->id;
        comic->author = updatedKomik->author;
        comic->genre = updatedKomik->genre;

        delete updatedKomik; // Hapus komik baru (sudah di-copy)
    }

    return true;
}

// ===== TRAVERSAL: PRE-ORDER =====
// Urutan: Root → Left → Right
void BST::preOrder(function<void(Komik *)> callback) const
{
    // function<void(Komik*)> callback = fungsi yang dikirim sebagai parameter
    // Contoh: tree.preOrder([](Komik* k) { k->display(); });

    preOrderHelper(root, callback);
}

void BST::preOrderHelper(Komik *node, function<void(Komik *)> callback) const
{
    if (node == nullptr)
    {
        return; // Base case: node kosong
    }

    // Urutan PRE-ORDER:
    callback(node);                        // 1. Proses ROOT dulu
    preOrderHelper(node->left, callback);  // 2. Proses LEFT subtree
    preOrderHelper(node->right, callback); // 3. Proses RIGHT subtree
}

// ===== TRAVERSAL: IN-ORDER =====
// Urutan: Left → Root → Right
// PENTING: Hasil IN-ORDER = data TERURUT (sorted)!
void BST::inOrder(function<void(Komik *)> callback) const
{
    inOrderHelper(root, callback);
}

void BST::inOrderHelper(Komik *node, function<void(Komik *)> callback) const
{
    if (node == nullptr)
    {
        return;
    }

    // Urutan IN-ORDER:
    inOrderHelper(node->left, callback);  // 1. Proses LEFT dulu
    callback(node);                       // 2. Proses ROOT
    inOrderHelper(node->right, callback); // 3. Proses RIGHT terakhir

    // Kenapa hasil terurut?
    // Karena di BST: LEFT < ROOT < RIGHT
    // Jadi kalau kita proses left dulu, terus root, terus right,
    // hasilnya pasti urut dari kecil ke besar!
}

// ===== TRAVERSAL: POST-ORDER =====
// Urutan: Left → Right → Root
void BST::postOrder(function<void(Komik *)> callback) const
{
    postOrderHelper(root, callback);
}

void BST::postOrderHelper(Komik *node, function<void(Komik *)> callback) const
{
    if (node == nullptr)
    {
        return;
    }

    // Urutan POST-ORDER:
    postOrderHelper(node->left, callback);  // 1. Proses LEFT
    postOrderHelper(node->right, callback); // 2. Proses RIGHT
    callback(node);                         // 3. Proses ROOT terakhir

    // Kegunaan: untuk delete tree (kita pakai di destroyTree)
}

// ===== IS EMPTY =====
// Cek apakah tree kosong
bool BST::isEmpty() const
{
    return root == nullptr; // Jika root null, tree kosong
}

// ===== COUNT =====
// Hitung total node di tree
int BST::count() const
{
    return countHelper(root);
}

int BST::countHelper(Komik *node) const
{
    if (node == nullptr)
    {
        return 0; // Node kosong = 0
    }

    // Rumus rekursif:
    // Total = 1 (node ini) + jumlah di left + jumlah di right
    return 1 + countHelper(node->left) + countHelper(node->right);
}

// ===== CLEAR =====
// Hapus semua node di tree
void BST::clear()
{
    destroyTree(root); // Hapus semua node
    root = nullptr;    // Set root jadi null (tree kosong)
}

// ===== GET ALL KOMIKS =====
// Ambil semua komik dalam bentuk vector
vector<Komik *> BST::getAllKomiks() const
{
    vector<Komik *> comics; // Bikin vector kosong

    // Pakai inOrder untuk ambil semua komik terurut
    inOrder([&comics](Komik *comic)
            {
                // [&comics] = lambda capture, tangkap variable comics by reference
                // Artinya: fungsi lambda ini bisa akses dan ubah variable comics

                comics.push_back(comic); // Masukkan comic ke vector
            });

    return comics; // Return vector berisi semua komik
}

/*
 * ===== RINGKASAN BST.CPP =====
 *
 * 1. CONSTRUCTOR/DESTRUCTOR:
 *    - Constructor: set root = nullptr (tree kosong)
 *    - Destructor: hapus semua node (cleanup memory)
 *
 * 2. INSERT:
 *    - Rekursif: bandingkan dengan node saat ini
 *    - Jika lebih kecil → kiri, jika lebih besar → kanan
 *    - Jika sama → update data
 *
 * 3. SEARCH:
 *    - Rekursif: bandingkan title
 *    - Jika lebih kecil → cari kiri, lebih besar → cari kanan
 *    - Jika sama → return node
 *
 * 4. DELETE:
 *    - 3 kasus: leaf (0 anak), 1 anak, 2 anak
 *    - Kasus 2 anak paling tricky: pakai successor
 *
 * 5. TRAVERSAL:
 *    - Pre-order: Root → Left → Right
 *    - In-order: Left → Root → Right (SORTED!)
 *    - Post-order: Left → Right → Root
 *
 * 6. UTILITY:
 *    - isEmpty, count, clear, getAllKomiks
 *
 * 7. KONSEP PENTING:
 *    - Rekursi: fungsi memanggil dirinya sendiri
 *    - Base case: kondisi berhenti
 *    - Recursive case: panggil fungsi lagi dengan input berbeda
 */