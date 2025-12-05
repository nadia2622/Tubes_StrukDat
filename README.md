# Komik Management System

### Tugas Besar Struktur Data - Binary Search Tree

---

## 📋 Deskripsi Project

Project ini mengimplementasikan **Binary Search Tree (BST)** untuk sistem manajemen komik. Fitur yang diimplementasikan meliputi:

- ✅ Load sample data ke dalam Tree
- ✅ CRUD Komik (Create, Read, Update, Delete)
- ✅ CRUD Genre & Author
- ✅ Implementasi Traversal (Pre-order, In-order, Post-order)
- ✅ View Favorites dari semua user

---

## 🗂️ Struktur Folder

```
KomikManagementSystem/
│
├── include/
│   ├── Komik.h           # Node structure
│   ├── BST.h             # BST header
│   └── KomikManager.h    # Manager (ganti Database)
|   └── SearchFilter.h    # untuk fitur pencarian berdasarkan title, author, genre
│
├── src/
│   ├── BST.cpp           # BST implementation
│   └── KomikManager.cpp  # Manager implementation
│   └── SearchFilter.cpp  # SearchFilter implementation
│
├── main.cpp              # Program utama
└── README.md             # Dokumentasi ini
```

---

## 💻 Cara Compile & Run

### Method 1: Pakai Extension C/C++ Compile Run (Yang aku pake sekarang)

1. Install Extension **C/C++ Compile Run**
2. Langsung menuju file main.cpp
3. Jalankan menggunakan icon run segitiga di kanan atas dan pilih **C/C++ Compile & Run**

### Method 2: Manual Compile

```cmd
# Navigate ke folder project
cd KomikManagementSystem

# Compile
g++ -o komik_system main.cpp src/BST.cpp src/KomikManager.cpp -I include -std=c++11

# Run
komik_system.exe
```

## 📚 Penjelasan Kode Per File

### 1. **Komik.h** - Node Structure

```cpp
struct Komik {
    int id;              // ID komik
    string title;        // Judul komik (key untuk sorting)
    string author;       // Pengarang
    string genre;        // Genre
    Komik* left;         // Left child (BST)
    Komik* right;        // Right child (BST)
};
```

**Sorting:** title → author → genre (hierarchical)

### 2. **BST.h/cpp** - Binary Search Tree

Implementasi lengkap BST dengan operasi:

- **Insert**: O(log n) average
- **Search**: O(log n) average
- **Delete**: O(log n) average (3 kasus: leaf, 1 child, 2 children)
- **Traversal**: O(n) - Pre/In/Post-order

### 3. **KomikManager.h/cpp** - Data Manager

Menggantikan Database class dengan:

- Sample data hardcoded (10 komik)
- In-memory storage untuk genres, authors, favorites
- Auto-increment ID

### 4. **main.cpp** - Program Utama

Menu interaktif lengkap:

- CRUD Komik
- CRUD Genre & Author
- Traversal Tree
- View Favorites

---

## 🎯 Fitur yang Diimplementasikan

### ✅ **1. Load Sample Data**

10 komik sample otomatis diload saat program start:

- Naruto, One Piece, Dragon Ball, dll
- 8 genre default
- 8 author default

### ✅ **2. CRUD Komik**

- **Add**: Input title, author, genre → insert ke BST
- **View**: In-order traversal (sorted by title)
- **Search**: Search by title (O(log n))
- **Update**: Edit title/author/genre
- **Delete**: Remove dari BST

### ✅ **3. CRUD Genre**

- Add, Update, Delete genre
- Auto-update comics yang pakai genre tersebut

### ✅ **4. CRUD Author**

- Add, Update, Delete author
- Auto-update comics yang pakai author tersebut

### ✅ **5. Traversal**

- **Pre-order**: Root → Left → Right
- **In-order**: Left → Root → Right (sorted!)
- **Post-order**: Left → Right → Root

---

## 🐛 Troubleshooting

### Error: `g++: command not found`

**Solusi:** Install MinGW/GCC

1. Download MSYS2: https://www.msys2.org/
2. Install dan buka MSYS2 terminal
3. Run: `pacman -S mingw-w64-x86_64-gcc`
4. Add to PATH: `C:\msys64\mingw64\bin`

### Error: `cannot open source file`

**Solusi:** Pastikan struktur folder benar

```
KomikManagementSystem/
├── include/
│   ├── Komik.h
│   ├── BST.h
│   └── KomikManager.h
├── src/
│   ├── BST.cpp
│   └── KomikManager.cpp
└── main.cpp
```

### Program Crash

**Solusi:** Check error message. Biasanya:

- Segmentation fault → memory issue (sudah di-handle di destructor)
- Invalid input → gunakan try-catch

---

## 📊 Contoh Output

### Main Menu

```
==============================================
   MAIN MENU
==============================================

1. CRUD Komik
2. CRUD Genre
3. CRUD Author
4. Traversal Tree
5. View Favorites (All Users)
6. Reload Sample Data
0. Exit

Choice: _
```

### View All Komik (In-Order)

```
ID    Title                          Author                    Genre
---------------------------------------------------------------------------
4     Attack on Titan                Hajime Isayama           Action
5     Berserk                        Kentaro Miura            Fantasy
8     Bleach                         Tite Kubo                Action
6     Death Note                     Tsugumi Ohba             Mystery
3     Dragon Ball                    Akira Toriyama           Action
9     Fullmetal Alchemist            Hiromu Arakawa           Fantasy
7     Hunter x Hunter                Yoshihiro Togashi        Action
1     Naruto                         Masashi Kishimoto        Action
2     One Piece                      Eiichiro Oda             Action
10    Tokyo Ghoul                    Sui Ishida               Horror

Total: 10 comics
```

### Contoh Traversal

```
PRE-ORDER TRAVERSAL
Order: Root -> Left -> Right

ID: 1 | Title: Naruto | Author: Masashi Kishimoto | Genre: Action
ID: 4 | Title: Attack on Titan | Author: Hajime Isayama | Genre: Action
ID: 5 | Title: Berserk | Author: Kentaro Miura | Genre: Fantasy
...
```

---

## 📝 Untuk Laporan

### Konsep yang Dipelajari

1. **Binary Search Tree**

   - Node structure dengan left/right pointer
   - Sorting berdasarkan key (title)
   - BST property: left < root < right

2. **Tree Traversal**

   - Pre-order: untuk copy tree
   - In-order: untuk sorted output
   - Post-order: untuk delete tree

3. **CRUD Operations**

   - Implemented langsung di BST
   - No database dependency

4. **Memory Management**
   - Dynamic allocation (`new`)
   - Proper cleanup (`delete`)
   - Destructor untuk prevent memory leak

---

## 👥 Pembagian Tugas

### Geusan

- ✅ Desain Node
- ✅ Load sample data ke Tree
- ✅ CRUD (Komik, Genre, Author)
- ✅ Traversal (Pre/In/Post-order)
- ✅ View Favorites

### Nadia

- ✅ Search by title (DFS)
- ✅ Filter by Genre
- ✅ Filter by Author
- Desain Tree

### Ryvanda (Pending)

- Menu Interaksi
- Authentication
- Role Management
- Laporan

---

## 💡 Catatan Penting

1. **Data tersimpan di memory**: Semua data hilang saat program close. Untuk sementara kita gak pake database, dikarenakan masih error brutal.

2. **Sample Data (Optional)**: 10 komik otomatis diload saat program start. Bisa reload dengan menu "Reload Sample Data".

3. **ID Auto-Increment**: ID dimulai dari 1 dan terus bertambah. Tidak akan reset kecuali program di-restart.

4. **Memory Management**: BST destructor otomatis cleanup semua node. No memory leak!

5. **Apa itu memory leak?**: Memory leak adalah kondisi di mana memori digunakan terus tanpa dilepas, sehingga memakan RAM dan menyebabkan performa menurun.

6. **Future: Integrasi Database**: Jika nanti mau tambah/ganti dengan database, tinggal ganti `KomikManager` dengan `Database` class.

---

**Good luck! Semoga lancar! 🚀**
