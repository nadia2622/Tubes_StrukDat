/*
 * main.cpp - PART 1
 * Program utama untuk testing sistem manajemen komik
 *
 * File ini berisi:
 * - Fungsi main()
 * - Fungsi-fungsi helper (clearScreen, pause, printHeader)
 * - Fungsi menu utama
 */

// ===== INCLUDE HEADER FILES =====
#include "include/Komik.h"        // Include struct Komik
#include "include/BST.h"          // Include class BST
#include "include/KomikManager.h" // Include class KomikManager
#include "src/BST.cpp"            // Include implementasi BST
#include "src/KomikManager.cpp"   // Include implementasi KomikManager
#include "include/SearchFilter.h" // Include class SearchFilter
#include "src/SearchFilter.cpp"   // Include implementasi SearchFilter
// CATATAN: Biasanya tidak include .cpp, tapi untuk simplicity di sini kita include

#include <iostream> // Untuk pakai cout, cin
#include <iomanip>  // Untuk pakai setw (set width) - format output
#include <limits>   // Untuk pakai numeric_limits

using namespace std; // Biar tidak perlu tulis std:: terus

// ===== FUNCTION PROTOTYPES (deklarasi fungsi) =====
// Ini seperti "daftar isi" fungsi yang ada di file ini
// Definisi (isi) fungsinya ada di bawah

void clearScreen();                    // Fungsi untuk clear layar terminal
void pause();                          // Fungsi untuk pause program (tunggu user tekan Enter)
void printHeader(const string &title); // Fungsi untuk print header dengan border

// Fungsi-fungsi menu
void mainMenu(BST &tree, KomikManager &manager);
void crudKomikMenu(BST &tree, KomikManager &manager);
void crudGenreMenu(KomikManager &manager, BST &tree);
void crudAuthorMenu(KomikManager &manager, BST &tree);
void traversalMenu(BST &tree);
void viewFavoritesMenu(KomikManager &manager, BST &tree);

// ===== FUNGSI MAIN =====
// Fungsi main adalah "pintu masuk" program
// Program dimulai dari sini
int main()
{
    // ===== PRINT HEADER PROGRAM =====
    // \033[1;36m = ANSI escape code untuk warna cyan + bold
    // \033[0m = reset warna ke default
    cout << "\033[1;36m==============================================\n";
    cout << "   KOMIK MANAGEMENT SYSTEM\n";
    cout << "==============================================\033[0m\n"
         << endl;

    // ===== INISIALISASI OBJEK =====

    // Bikin objek KomikManager
    // Constructor KomikManager() dipanggil otomatis
    KomikManager manager;

    // Bikin objek BST
    // Constructor BST() dipanggil otomatis
    BST tree;

    // ===== LOAD SAMPLE DATA (opsional) =====
    // Baris ini di-comment karena mau mulai dengan tree kosong
    // Jika mau auto-load sample data, uncomment 2 baris di bawah
    // cout << "Loading sample comics...\n" << endl;
    // manager.loadSampleData(tree);

    // ===== PRINT MESSAGE =====
    cout << "\033[32mSystem ready!\033[0m" << endl; // \033[32m = warna hijau
    pause();                                        // Tunggu user tekan Enter

    // ===== JALANKAN MAIN MENU =====
    mainMenu(tree, manager);
    // Fungsi ini akan loop terus sampai user pilih exit

    // ===== EXIT MESSAGE =====
    cout << "\033[36m\nThank you for using Komik Management System!\033[0m\n";

    return 0; // Return 0 = program berakhir sukses
}

// ===== FUNGSI CLEAR SCREEN =====
// Fungsi untuk clear layar terminal
void clearScreen()
{
// #ifdef = preprocessor directive, cek apakah macro _WIN32 ada
#ifdef _WIN32
    // Jika Windows, pakai command "cls"
    system("cls");
#else
    // Jika Linux/Mac, pakai command "clear"
    system("clear");
#endif
    // system() = jalankan command di terminal
}

// ===== FUNGSI PAUSE =====
// Fungsi untuk pause program sampai user tekan Enter
void pause()
{
    cout << "\nTekan Enter untuk lanjut...";

    // cin.ignore() = abaikan semua karakter di input buffer sampai newline
    // Kenapa perlu? Karena setelah cin >> choice, masih ada newline di buffer
    // numeric_limits<streamsize>::max() = jumlah maksimal karakter yang diabaikan
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // cin.get() = tunggu user tekan Enter
    cin.get();
}

// ===== FUNGSI PRINT HEADER =====
// Fungsi untuk print header dengan border
void printHeader(const string &title)
{
    clearScreen(); // Clear screen dulu

    // Print border atas
    cout << "\033[1;36m"; // Warna cyan + bold
    cout << "==============================================\n";
    cout << "   " << title << "\n"; // Print title
    cout << "==============================================\033[0m\n"
         << endl;
    // \033[0m = reset warna
}

// ===== FUNGSI MAIN MENU =====
// Ini adalah menu utama program
void mainMenu(BST &tree, KomikManager &manager)
{
    // BST& tree = reference ke tree (tidak copy)
    // KomikManager& manager = reference ke manager

    int choice; // Variable untuk simpan pilihan user

    // ===== LOOP MENU =====
    // do-while = loop yang pasti jalan minimal 1 kali
    do
    {
        printHeader("MAIN MENU"); // Print header

        // ===== PRINT PILIHAN MENU =====
        cout << "1. CRUD Komik\n";
        cout << "2. CRUD Genre\n";
        cout << "3. CRUD Author\n";
        cout << "4. Traversal Tree\n";
        cout << "5. View Favorites (All Users)\n";
        cout << "6. Reload Sample Data\n";
        cout << "0. Exit\n";
        cout << "\nChoice: ";

        // ===== INPUT PILIHAN USER =====
        cin >> choice;
        // cin >> = baca input dari keyboard

        // ===== ERROR HANDLING INPUT =====
        // Jika user input bukan angka (misal: huruf), cin.fail() = true
        if (cin.fail())
        {
            cin.clear();                                         // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Abaikan input salah
            choice = -1;                                         // Set choice jadi -1 (invalid)
        }

        // ===== SWITCH CASE =====
        // switch = if-else yang lebih rapi untuk banyak kondisi
        switch (choice)
        {
        case 1:
            // Jika pilih 1, panggil menu CRUD Komik
            crudKomikMenu(tree, manager);
            break; // break = keluar dari switch

        case 2:
            // Jika pilih 2, panggil menu CRUD Genre
            crudGenreMenu(manager, tree);
            break;

        case 3:
            // Jika pilih 3, panggil menu CRUD Author
            crudAuthorMenu(manager, tree);
            break;

        case 4:
            // Jika pilih 4, panggil menu Traversal
            traversalMenu(tree);
            break;

        case 5:
            // Jika pilih 5, panggil menu View Favorites
            viewFavoritesMenu(manager, tree);
            break;

        case 6:
            // Jika pilih 6, reload sample data
            printHeader("RELOAD SAMPLE DATA");
            manager.loadSampleData(tree); // Load sample data
            cout << "\033[32mSample data reloaded!\033[0m" << endl;
            pause();
            break;

        case 0:
            // Jika pilih 0, exit
            cout << "\nExiting..." << endl;
            break;

        default:
            // default = jika tidak ada case yang match
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0); // Loop terus sampai user pilih 0 (exit)
}

/*
 * ===== RINGKASAN main.cpp PART 1 =====
 *
 * 1. FUNGSI main():
 *    - Bikin objek KomikManager dan BST
 *    - (Opsional) Load sample data
 *    - Panggil mainMenu()
 *
 * 2. FUNGSI clearScreen():
 *    - Windows: system("cls")
 *    - Linux/Mac: system("clear")
 *
 * 3. FUNGSI pause():
 *    - cin.ignore() untuk clear buffer
 *    - cin.get() untuk tunggu Enter
 *
 * 4. FUNGSI printHeader():
 *    - Clear screen
 *    - Print border + title dengan warna
 *
 * 5. FUNGSI mainMenu():
 *    - Loop do-while sampai user pilih exit
 *    - Print menu
 *    - Input pilihan user
 *    - Error handling jika input salah
 *    - Switch case untuk panggil fungsi menu sesuai pilihan
 *
 * 6. KONSEP PENTING:
 *    - do-while: loop yang pasti jalan minimal 1 kali
 *    - switch-case: if-else yang lebih rapi
 *    - cin.fail(): cek apakah input error
 *    - cin.clear(): clear error flag
 *    - cin.ignore(): abaikan karakter di buffer
 *    - ANSI escape codes: untuk warna di terminal
 */

/*
 * main.cpp - PART 2
 * CRUD KOMIK MENU
 *
 * Fungsi ini menangani semua operasi CRUD untuk komik:
 * - Create (Add)
 * - Read (View All, Search)
 * - Update
 * - Delete
 */

// ===== FUNGSI CRUD KOMIK MENU =====
void crudKomikMenu(BST &tree, KomikManager &manager)
{
    int choice;
    SearchFilter filter;

    do
    {
        printHeader("CRUD COMIC");

        // Print info jumlah komik di tree
        cout << "Total comics in tree: " << tree.count() << "\n\n";

        // Print menu
        cout << "1. Add Komik\n";
        cout << "2. View All Komiks (In-Order)\n";
        cout << "3. Search Komik by Title\n";
        cout << "4. Filter Komik by Author\n";
        cout << "5. Filter Komik by Genre\n";
        cout << "6. Update Komik\n";
        cout << "7. Delete Komik\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore(); // Abaikan newline setelah cin >>
        // PENTING: cin.ignore() di sini supaya getline() di bawah tidak skip

        // Error handling
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        // ===== CASE 1: ADD KOMIK =====
        case 1:
        {
            // { } = membuat scope baru
            // Kenapa perlu? Karena kita deklarasi variable baru di dalam case
            // Tanpa { }, variable akan conflict dengan case lain

            printHeader("ADD COMIC");

            // Deklarasi variable untuk simpan input
            string title, author, genre;

            // ===== INPUT TITLE =====
            cout << "Enter title: ";
            getline(cin, title);
            // getline(cin, title) = baca satu baris input (sampai Enter)
            // Kenapa pakai getline? Karena title bisa ada spasi
            // Contoh: "Attack on Titan" (ada spasi)

            // ===== INPUT AUTHOR =====
            cout << "Enter author: ";
            getline(cin, author);

            // ===== TAMPILKAN AVAILABLE GENRES =====
            cout << "\nAvailable genres:\n";
            vector<string> genres = manager.getAllGenres();
            // Ambil semua genre dari manager

            // Loop untuk print semua genre
            // size_t = tipe data unsigned int untuk ukuran
            for (size_t i = 0; i < genres.size(); i++)
            {
                cout << (i + 1) << ". " << genres[i] << endl;
                // Print nomor dan nama genre
            }

            // ===== INPUT GENRE =====
            cout << "\nEnter genre: ";
            getline(cin, genre);

            // ===== INSERT KE TREE =====
            // Get ID baru dari manager
            int newId = manager.getNextId();

            // Bikin node Komik baru dengan new
            Komik *newKomik = new Komik(newId, title, author, genre);

            // Insert ke tree
            tree.insert(newKomik);

            // Print success message
            cout << "\033[32mKomik added successfully with ID: "
                 << newId << "\033[0m" << endl;

            pause();
            break;
        }

        // ===== CASE 2: VIEW ALL KOMIKS =====
        case 2:
        {
            printHeader("ALL COMICS (IN-ORDER TRAVERSAL)");

            // Cek apakah tree kosong
            if (tree.isEmpty())
            {
                cout << "\033[33mNo comics found!\033[0m" << endl;
            }
            else
            {
                // ===== PRINT HEADER TABEL =====
                // setw(n) = set width (lebar kolom) = n karakter
                // left = align kiri
                cout << left << setw(5) << "ID"
                     << setw(30) << "Title"
                     << setw(25) << "Author"
                     << setw(15) << "Genre" << endl;

                // Print garis pembatas
                // string(75, '-') = bikin string berisi 75 karakter '-'
                cout << string(75, '-') << endl;

                // ===== PRINT SEMUA KOMIK =====
                // Pakai inOrder traversal (hasil terurut by title)
                tree.inOrder([](Komik *comic)
                             {
                        // Lambda function: [](parameter) { body }
                        // [] = lambda capture (kosong = tidak tangkap variable luar)
                        // Komik* comic = parameter
                        
                        // Print data komik dalam format tabel
                        cout << left << setw(5) << comic->id
                             << setw(30) << comic->title
                             << setw(25) << comic->author
                             << setw(15) << comic->genre << endl; });

                // Print total
                cout << "\nTotal: " << tree.count() << " comics" << endl;
            }

            pause();
            break;
        }

        // ===== CASE 3: SEARCH KOMIK =====
        case 3:
        {
            printHeader("SEARCH COMIC BY TITLE");

            string keyword;
            cout << "Enter title keyword: ";
            getline(cin, keyword);

            vector<Komik*> results = filter.searchByTitle(tree, keyword);

            if (results.empty()) {
                cout << "\033[31mNo comics found!\033[0m\n";
            } else {
                cout << "\n\033[32m" << results.size() << " comics found:\033[0m\n\n";

                cout << left << setw(5) << "ID"
                    << setw(30) << "Title"
                    << setw(25) << "Author"
                    << setw(15) << "Genre" << endl;
                cout << string(75, '-') << endl;

                for (Komik* k : results) {
                    cout << left << setw(5) << k->id
                        << setw(30) << k->title
                        << setw(25) << k->author
                        << setw(15) << k->genre << endl;
                }
            }
            pause();
            break;
        }

        // ===== CASE 4: FILTER KOMIK BY AUTHOR =====
        case 4:
        {
            printHeader("FILTER COMIC BY AUTHOR");

            string keyword;
            cout << "Enter author keyword: ";
            getline(cin, keyword);

            vector<Komik*> results = filter.searchByAuthor(tree, keyword);

            if (results.empty()) {
                cout << "\033[31mNo comics found!\033[0m\n";
            } else {
                cout << "\n\033[32m" << results.size() << " comics found:\033[0m\n\n";

                cout << left << setw(5) << "ID"
                    << setw(30) << "Title"
                    << setw(25) << "Author"
                    << setw(15) << "Genre" << endl;
                cout << string(75, '-') << endl;

                for (Komik* k : results) {
                    cout << left << setw(5) << k->id
                        << setw(30) << k->title
                        << setw(25) << k->author
                        << setw(15) << k->genre << endl;
                }
            }
            pause();
            break;
        }

        // ===== CASE 5: FILTER KOMIK BY GENRE =====
        case 5:
        {
            printHeader("FILTER KOMIK BY GENRE");

            cin.ignore();
            string keyword;
            cout << "Enter genre keyword: ";
            getline(cin, keyword);

            vector<Komik*> results = filter.searchByGenre(tree, keyword);

            if (results.empty()) {
                cout << "\033[31mNo comics found!\033[0m\n";
            } else {
                cout << "\n\033[32m" << results.size() << " comics found:\033[0m\n\n";

                cout << left << setw(5) << "ID"
                    << setw(30) << "Title"
                    << setw(25) << "Author"
                    << setw(15) << "Genre" << endl;
                cout << string(75, '-') << endl;

                for (Komik* k : results) {
                    cout << left << setw(5) << k->id
                        << setw(30) << k->title
                        << setw(25) << k->author
                        << setw(15) << k->genre << endl;
                }
            }

            pause();
            break;
        }

        // ===== CASE 6: UPDATE KOMIK =====
        case 6:
        {
            printHeader("UPDATE COMIC");

            string oldTitle;
            cout << "Enter title of comic to update: ";
            getline(cin, oldTitle);

            // Cari komik by title
            Komik *found = tree.search(oldTitle);

            if (found == nullptr)
            {
                // Komik tidak ditemukan
                cout << "\033[31mKomik not found!\033[0m" << endl;
                pause();
                break; // Keluar dari case 4
            }

            // ===== TAMPILKAN DATA LAMA =====
            cout << "\nCurrent data:\n";
            cout << "Title: " << found->title << endl;
            cout << "Author: " << found->author << endl;
            cout << "Genre: " << found->genre << endl;

            // ===== INPUT DATA BARU =====
            string newTitle, newAuthor, newGenre;

            cout << "\nEnter new data:\n";
            cout << "Title: ";
            getline(cin, newTitle);
            cout << "Author: ";
            getline(cin, newAuthor);
            cout << "Genre: ";
            getline(cin, newGenre);

            // ===== UPDATE TREE =====
            // Bikin node komik baru dengan data baru
            Komik *updatedKomik = new Komik(found->id, newTitle, newAuthor, newGenre);

            // Update tree
            tree.update(oldTitle, updatedKomik);

            cout << "\033[32mKomik updated successfully!\033[0m" << endl;
            pause();
            break;
        }

        // ===== CASE 7: DELETE KOMIK =====
        case 7:
        {
            printHeader("DELETE COMIC");

            string title;
            cout << "Enter title of comic to delete: ";
            getline(cin, title);

            // Cari komik by title
            Komik *found = tree.search(title);

            if (found == nullptr)
            {
                cout << "\033[31mKomik not found!\033[0m" << endl;
                pause();
                break;
            }

            // ===== KONFIRMASI DELETE =====
            cout << "\nAre you sure you want to delete this comic?\n";
            found->display(); // Tampilkan info komik
            cout << "\n(y/n): ";

            char confirm;
            cin >> confirm;
            // Hanya baca 1 karakter (y atau n)

            // Cek konfirmasi
            if (confirm == 'y' || confirm == 'Y')
            {
                // Jika yes, hapus dari tree
                tree.remove(title);
                cout << "\033[32mKomik deleted successfully!\033[0m" << endl;
            }
            else
            {
                // Jika no, cancel
                cout << "Delete cancelled." << endl;
            }

            pause();
            break;
        }

        case 0:
            // Back ke main menu
            break;

        default:
            cout << "\033[31mInvalid choice!\033[0m" << endl;
            pause();
        }
    } while (choice != 0);
}

/*
 * ===== RINGKASAN CRUD KOMIK MENU =====
 *
 * 1. ADD KOMIK (Case 1):
 *    - Input: title, author, genre
 *    - Tampilkan list available genres
 *    - Get ID baru dari manager
 *    - Bikin node baru dengan new
 *    - Insert ke tree
 *
 * 2. VIEW ALL (Case 2):
 *    - Cek apakah tree kosong
 *    - Pakai inOrder traversal untuk print semua komik
 *    - Lambda function untuk callback
 *    - Format output pakai setw() (tabel rapi)
 *
 * 3. SEARCH (Case 3):
 *    - Input title
 *    - Pakai tree.search()
 *    - Cek apakah nullptr (tidak ketemu)
 *    - Print info jika ketemu
 *
 * 4. UPDATE (Case 4):
 *    - Cari komik by title
 *    - Tampilkan data lama
 *    - Input data baru
 *    - Bikin node baru dengan data baru
 *    - Pakai tree.update()
 *
 * 5. DELETE (Case 5):
 *    - Cari komik by title
 *    - Tampilkan info komik
 *    - Minta konfirmasi (y/n)
 *    - Jika yes, pakai tree.remove()
 *
 * 6. KONSEP PENTING:
 *    - getline(): baca input dengan spasi
 *    - Lambda function: [](){} untuk callback
 *    - setw(): format output (tabel rapi)
 *    - new: alokasi memory dinamis
 *    - nullptr: pointer kosong
 *    - Scope { }: untuk deklarasi variable di case
 */

/*
 * main.cpp - PART 3 (FINAL)
 * Menu-menu lainnya:
 * - CRUD Genre
 * - CRUD Author
 * - Traversal
 * - View Favorites
 */

// ===== FUNGSI CRUD GENRE MENU =====
// Fungsi ini hampir sama dengan CRUD Komik, tapi lebih sederhana
void crudGenreMenu(KomikManager &manager, BST &tree)
{
    int choice;

    do
    {
        printHeader("CRUD GENRE");

        cout << "1. View All Genres\n";
        cout << "2. Add Genre\n";
        cout << "3. Update Genre\n";
        cout << "4. Delete Genre\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore(); // Abaikan newline

        switch (choice)
        {
        // ===== CASE 1: VIEW ALL GENRES =====
        case 1:
        {
            printHeader("ALL GENRES");

            // Ambil semua genre dari manager
            vector<string> genres = manager.getAllGenres();

            if (genres.empty())
            {
                // empty() = cek apakah vector kosong
                cout << "\033[33mNo genres found!\033[0m" << endl;
            }
            else
            {
                cout << "Total genres: " << genres.size() << "\n\n";

                // Loop print semua genre
                for (size_t i = 0; i < genres.size(); i++)
                {
                    cout << (i + 1) << ". " << genres[i] << endl;
                }
            }

            pause();
            break;
        }

        // ===== CASE 2: ADD GENRE =====
        case 2:
        {
            printHeader("ADD GENRE");

            string name;
            cout << "Enter genre name: ";
            getline(cin, name);

            // Tambah genre lewat manager
            manager.addGenre(name);
            // Manager akan cek apakah genre sudah ada
            // Jika belum ada, tambah ke list

            pause();
            break;
        }

        // ===== CASE 3: UPDATE GENRE =====
        case 3:
        {
            printHeader("UPDATE GENRE");

            string oldName, newName;

            cout << "Enter old genre name: ";
            getline(cin, oldName);

            cout << "Enter new genre name: ";
            getline(cin, newName);

            // Update genre lewat manager
            manager.updateGenre(oldName, newName, tree);
            // Manager akan:
            // 1. Update nama genre di list
            // 2. Update genre di semua komik yang pakai genre ini

            pause();
            break;
        }

        // ===== CASE 4: DELETE GENRE =====
        case 4:
        {
            printHeader("DELETE GENRE");

            string name;
            cout << "Enter genre name to delete: ";
            getline(cin, name);

            // Minta konfirmasi
            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                manager.deleteGenre(name);
            }

            pause();
            break;
        }
        }
    } while (choice != 0);
}

// ===== FUNGSI CRUD AUTHOR MENU =====
// Fungsi ini IDENTIK dengan CRUD Genre
// Hanya beda: genres → authors
void crudAuthorMenu(KomikManager &manager, BST &tree)
{
    int choice;

    do
    {
        printHeader("CRUD AUTHOR");

        cout << "1. View All Authors\n";
        cout << "2. Add Author\n";
        cout << "3. Update Author\n";
        cout << "4. Delete Author\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            printHeader("ALL AUTHORS");
            vector<string> authors = manager.getAllAuthors();

            if (authors.empty())
            {
                cout << "\033[33mNo authors found!\033[0m" << endl;
            }
            else
            {
                cout << "Total authors: " << authors.size() << "\n\n";
                for (size_t i = 0; i < authors.size(); i++)
                {
                    cout << (i + 1) << ". " << authors[i] << endl;
                }
            }
            pause();
            break;
        }

        case 2:
        {
            printHeader("ADD AUTHOR");
            string name;
            cout << "Enter author name: ";
            getline(cin, name);
            manager.addAuthor(name);
            pause();
            break;
        }

        case 3:
        {
            printHeader("UPDATE AUTHOR");
            string oldName, newName;
            cout << "Enter old author name: ";
            getline(cin, oldName);
            cout << "Enter new author name: ";
            getline(cin, newName);
            manager.updateAuthor(oldName, newName, tree);
            pause();
            break;
        }

        case 4:
        {
            printHeader("DELETE AUTHOR");
            string name;
            cout << "Enter author name to delete: ";
            getline(cin, name);

            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                manager.deleteAuthor(name);
            }
            pause();
            break;
        }
        }
    } while (choice != 0);
}

// ===== FUNGSI TRAVERSAL MENU =====
// Menu untuk test 3 jenis traversal
void traversalMenu(BST &tree)
{
    int choice;

    do
    {
        printHeader("TREE TRAVERSAL");

        cout << "1. Pre-Order Traversal\n";
        cout << "2. In-Order Traversal\n";
        cout << "3. Post-Order Traversal\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";

        cin >> choice;

        switch (choice)
        {
        // ===== CASE 1: PRE-ORDER =====
        case 1:
            printHeader("PRE-ORDER TRAVERSAL");
            cout << "Order: Root -> Left -> Right\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                // Pakai preOrder dengan lambda function
                tree.preOrder([](Komik *comic)
                              {
                                  comic->display(); // Print info komik
                              });
            }

            pause();
            break;

        // ===== CASE 2: IN-ORDER =====
        case 2:
            printHeader("IN-ORDER TRAVERSAL");
            cout << "Order: Left -> Root -> Right (Sorted)\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                tree.inOrder([](Komik *comic)
                             { comic->display(); });
                // PENTING: In-order menghasilkan data TERURUT!
            }

            pause();
            break;

        // ===== CASE 3: POST-ORDER =====
        case 3:
            printHeader("POST-ORDER TRAVERSAL");
            cout << "Order: Left -> Right -> Root\n\n";

            if (tree.isEmpty())
            {
                cout << "\033[33mTree is empty!\033[0m" << endl;
            }
            else
            {
                tree.postOrder([](Komik *comic)
                               { comic->display(); });
            }

            pause();
            break;
        }
    } while (choice != 0);
}

// ===== FUNGSI VIEW FAVORITES MENU =====
// Fungsi untuk tampilkan favorit semua user
void viewFavoritesMenu(KomikManager &manager, BST &tree)
{
    printHeader("USER FAVORITES");

    // Ambil semua favorit dari manager
    // auto = otomatis detect tipe (map<string, vector<int>>)
    auto allFavorites = manager.getAllFavorites();

    if (allFavorites.empty())
    {
        cout << "\033[33mNo favorites found!\033[0m" << endl;
    }
    else
    {
        // ===== PRINT HEADER TABEL =====
        cout << left << setw(15) << "Username"
             << setw(10) << "Komik ID"
             << setw(30) << "Komik Title" << endl;
        cout << string(55, '-') << endl;

        // ===== LOOP SEMUA USER =====
        // for (const auto& userFav : allFavorites)
        // Artinya: untuk setiap pair<username, vector<ID>> di map
        for (const auto &userFav : allFavorites)
        {
            // userFav adalah pair<string, vector<int>>
            // .first = key (username)
            // .second = value (vector<int> berisi ID komik)

            string username = userFav.first;       // Ambil username
            vector<int> comicIds = userFav.second; // Ambil list ID komik

            // ===== LOOP SEMUA KOMIK FAVORIT USER INI =====
            for (int comicId : comicIds)
            {
                // Cari title komik by ID
                string comicTitle = "Unknown"; // Default jika tidak ketemu

                // Pakai inOrder untuk cari komik by ID
                tree.inOrder([&comicTitle, comicId](Komik *comic)
                             {
                    // [&comicTitle, comicId] = lambda capture
                    // & = capture by reference (bisa ubah comicTitle)
                    // comicId = capture by value (tidak bisa ubah)
                    
                    if (comic->id == comicId) {
                        comicTitle = comic->title;  // Update comicTitle
                    } });

                // Print row tabel
                cout << left << setw(15) << username
                     << setw(10) << comicId
                     << setw(30) << comicTitle << endl;
            }
        }

        cout << "\nTotal users with favorites: " << allFavorites.size() << endl;
    }

    pause();
}

/*
 * ===== RINGKASAN main.cpp PART 3 =====
 *
 * 1. CRUD GENRE MENU:
 *    - View: ambil dari manager.getAllGenres()
 *    - Add: manager.addGenre()
 *    - Update: manager.updateGenre() (update list + komik)
 *    - Delete: manager.deleteGenre()
 *
 * 2. CRUD AUTHOR MENU:
 *    - Sama seperti CRUD Genre
 *    - Hanya beda: genres → authors
 *
 * 3. TRAVERSAL MENU:
 *    - Pre-order: Root → Left → Right
 *    - In-order: Left → Root → Right (SORTED!)
 *    - Post-order: Left → Right → Root
 *    - Pakai lambda function untuk callback
 *
 * 4. VIEW FAVORITES MENU:
 *    - Ambil semua favorit dari manager
 *    - Loop semua user
 *    - Loop semua komik favorit per user
 *    - Cari title komik by ID pakai inOrder
 *    - Print dalam format tabel
 *
 * 5. KONSEP PENTING:
 *    - auto: otomatis detect tipe variable
 *    - for (const auto& x : container): range-based for loop
 *    - pair.first & pair.second: akses key-value di map
 *    - Lambda capture: [&var] = by reference, [var] = by value
 *    - empty(): cek apakah container kosong
 *    - size(): ukuran container
 *
 * ===== KESIMPULAN KESELURUHAN =====
 *
 * Program ini adalah implementasi lengkap Binary Search Tree untuk
 * sistem manajemen komik dengan fitur:
 *
 * 1. BST: Insert, Search, Delete, Update, Traversal
 * 2. CRUD: Comic, Genre, Author
 * 3. Favorites: Manage favorit per user
 * 4. Manager: Handle sample data & auto-increment ID
 *
 * KONSEP STRUKTUR DATA YANG DIPELAJARI:
 * - Binary Search Tree (BST)
 * - Rekursi (untuk operasi BST)
 * - Tree Traversal (Pre/In/Post-order)
 * - Vector (array dinamis)
 * - Map (dictionary)
 * - Lambda function (callback)
 *
 * KONSEP C++ YANG DIPELAJARI:
 * - Class & Struct
 * - Pointer & Reference
 * - Constructor & Destructor
 * - Operator overloading
 * - Memory management (new/delete)
 * - STL (vector, map, algorithm)
 * - Lambda expression
 * - Template (function<>)
 *
 * SEMOGA MEMBANTU BELAJAR! 🚀
 */