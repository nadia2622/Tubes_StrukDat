#include "../include/BST.h"
#include <iostream>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>
#include <iomanip>

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    destroyTree(root);
}

void BST::destroyTree(Komik *node)
{
    if (node == nullptr)
    {
        return;
    }

    // Recursive case: hapus left subtree dulu
    destroyTree(node->left);

    // Hapus right subtree
    destroyTree(node->right);

    // Urutan: kiri → kanan → tengah (post-order)
    // Kenapa? Karena kita tidak bisa hapus parent sebelum hapus children
    delete node;
}

void BST::insert(Komik *newKomik)
{
    root = insertHelper(root, newKomik);
}

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

Komik *BST::search(const string &title) const
{
    return searchHelper(root, title);
}

Komik *BST::searchHelper(Komik *node, const string &title) const
{
    if (node == nullptr)
        return nullptr;

    if (node->title == title)
        return node;

    if (title < node->title)
    {
        return searchHelper(node->left, title);
    }
    else
    {
        return searchHelper(node->right, title);
    }
}

// ===== NEW: TO LOWERCASE HELPER =====
string BST::toLowerCase(const string &str) const
{
    string result = str;
    for (char &c : result)
    {
        c = tolower(c);
    }
    return result;
}

// ===== NEW: SEARCH PARTIAL (case-insensitive & partial match) =====
vector<Komik *> BST::searchPartial(const string &keyword) const
{
    vector<Komik *> results;
    searchPartialHelper(root, keyword, results);
    return results;
}

// ===== NEW: SEARCH PARTIAL HELPER =====
void BST::searchPartialHelper(Komik *node, const string &keyword, vector<Komik *> &results) const
{
    if (node == nullptr)
        return;

    // Convert title dan keyword ke lowercase untuk perbandingan
    string lowerTitle = toLowerCase(node->title);
    string lowerKeyword = toLowerCase(keyword);

    // Cek apakah keyword ada di dalam title (partial match)
    if (lowerTitle.find(lowerKeyword) != string::npos)
    {
        results.push_back(node);
    }

    // Tetap search di left dan right (traverse semua node)
    searchPartialHelper(node->left, keyword, results);
    searchPartialHelper(node->right, keyword, results);
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
            delete node;
            return nullptr;
        }

        // KASUS 2A: Node punya 1 anak (anak KANAN saja)
        else if (node->left == nullptr)
        {
            Komik *temp = node->right;
            delete node;
            return temp;
        }

        // KASUS 2B: Node punya 1 anak (anak KIRI saja)
        else if (node->right == nullptr)
        {
            Komik *temp = node->left;
            delete node;
            return temp;
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
    inOrderHelper(node->left, callback);
    callback(node);
    inOrderHelper(node->right, callback);
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
    postOrderHelper(node->left, callback);
    postOrderHelper(node->right, callback);
    callback(node);

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
        return 0;
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

vector<Komik *> BST::getAllKomiks() const
{
    vector<Komik *> comics;

    inOrder([&comics](Komik *comic)
            { comics.push_back(comic); });

    return comics; // Return vector berisi semua komik
}

// ===== FIND BY KEYWORD (Interactive Helper with Clear UI) =====
Komik *BST::findByKeyword(const string &keyword) const
{
    // Search partial
    vector<Komik *> results = searchPartial(keyword);

    // Kalau tidak ada hasil
    if (results.empty())
    {
        cout << "\n\033[31mTidak ada komik dengan keyword: '" << keyword << "'\033[0m" << endl;
        return nullptr;
    }

    // ===== SMART SORTING =====
    string lowerKeyword = toLowerCase(keyword);

    sort(results.begin(), results.end(), [&lowerKeyword, this](Komik *a, Komik *b)
         {
        string lowerA = toLowerCase(a->title);
        string lowerB = toLowerCase(b->title);
        
        // 1. Exact match
        bool exactA = (lowerA == lowerKeyword);
        bool exactB = (lowerB == lowerKeyword);
        if (exactA != exactB) return exactA;
        
        // 2. Starts with
        bool startsA = (lowerA.find(lowerKeyword) == 0);
        bool startsB = (lowerB.find(lowerKeyword) == 0);
        if (startsA != startsB) return startsA;
        
        // 3. Shorter length
        if (a->title.length() != b->title.length()) {
            return a->title.length() < b->title.length();
        }
        
        // 4. Alphabetical
        return lowerA < lowerB; });

    // ===== KALAU CUMA 1 HASIL =====
    if (results.size() == 1)
    {
        cout << "\n\033[32mFound: " << results[0]->title << "\033[0m" << endl;
        return results[0];
    }

    // ===== KALAU LEBIH DARI 1, USER HARUS PILIH =====
    cout << "\n\033[33mMultiple results found for: '" << keyword << "'\033[0m" << endl;

    cout << "\033[1;36mPilih salah satu komik:\033[0m\n"
         << endl;

    // Tampilkan dengan format tabel
    cout << left << setw(5) << "No."
         << setw(35) << "Title"
         << setw(25) << "Author" << endl;
    cout << string(75, '-') << endl;

    for (size_t i = 0; i < results.size(); i++)
    {
        cout << left << setw(5) << (i + 1)
             << setw(35) << results[i]->title
             << setw(25) << results[i]->author;

        cout << endl;
    }

    cout << string(75, '-') << endl;

    // ===== INPUT PILIHAN =====
    int choice;
    do
    {
        cout << "\n\033[1;33mPilih Angka (1-" << results.size() << "): \033[0m";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[31mInvalid input! Please enter a number.\033[0m" << endl;
            choice = -1;
            continue;
        }

        if (choice < 1 || choice > (int)results.size())
        {
            cout << "\033[31mInvalid choice! Please enter 1-" << results.size() << "\033[0m" << endl;
        }
    } while (choice < 1 || choice > (int)results.size());

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\033[32mSelected: " << results[choice - 1]->title << "\033[0m\n"
         << endl;

    return results[choice - 1];
}