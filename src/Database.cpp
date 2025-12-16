#include "../include/Database.h"
#include "../include/Auth.h"
#include <iostream>
#include <sstream>

using namespace std;

Database::Database() : db(nullptr), dbPath("") {}

Database::~Database()
{
    close();
}

bool Database::open(const string &path)
{
    dbPath = path;
    int rc = sqlite3_open(path.c_str(), &db);

    if (rc != SQLITE_OK)
    {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Database opened successfully: " << path << endl;
    return createTables();
}

void Database::close()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database connection closed." << endl;
    }
}

bool Database::isOpen() const
{
    return db != nullptr;
}

bool Database::createTables()
{
    const char *sql[] = {
        // Komiks table
        "CREATE TABLE IF NOT EXISTS komiks ("
        "id INTEGER PRIMARY KEY,"
        "title TEXT NOT NULL,"
        "author TEXT NOT NULL,"
        "genre TEXT NOT NULL);",

        // Genres table
        "CREATE TABLE IF NOT EXISTS genres ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE NOT NULL);",

        // Authors table
        "CREATE TABLE IF NOT EXISTS authors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE NOT NULL);",

        // Users table
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "role TEXT NOT NULL);",

        // Favorites table
        "CREATE TABLE IF NOT EXISTS favorites ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "komik_id INTEGER NOT NULL,"
        "FOREIGN KEY(username) REFERENCES users(username) ON DELETE CASCADE,"
        "FOREIGN KEY(komik_id) REFERENCES komiks(id) ON DELETE CASCADE,"
        "UNIQUE(username, komik_id));"};

    char *errMsg = nullptr;

    for (int i = 0; i < 5; i++)
    {
        int rc = sqlite3_exec(db, sql[i], nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
    }

    cout << "All tables created successfully." << endl;
    return true;
}

// ===== KOMIK OPERATIONS =====

bool Database::insertKomik(Komik *komik)
{
    if (!db || !komik)
        return false;

    const char *sql = "INSERT INTO komiks (id, title, author, genre) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, komik->id);
    sqlite3_bind_text(stmt, 2, komik->title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, komik->author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, komik->genre.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        cerr << "Failed to insert komik: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Komik inserted to database: " << komik->title << endl;
    return true;
}

bool Database::updateKomik(int id, Komik *komik)
{
    if (!db || !komik)
        return false;

    const char *sql = "UPDATE komiks SET title = ?, author = ?, genre = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, komik->title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, komik->author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, komik->genre.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        cerr << "Failed to update komik: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Komik updated in database: ID " << id << endl;
    return true;
}

bool Database::deleteKomik(int id)
{
    if (!db)
        return false;

    const char *sql = "DELETE FROM komiks WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        cerr << "Failed to delete komik: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "Komik deleted from database: ID " << id << endl;
    return true;
}

vector<Komik *> Database::getAllKomiks()
{
    vector<Komik *> komiks;
    if (!db)
        return komiks;

    const char *sql = "SELECT id, title, author, genre FROM komiks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return komiks;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        string title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string genre = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        Komik *k = new Komik(id, title, author, genre);
        komiks.push_back(k);
    }

    sqlite3_finalize(stmt);
    return komiks;
}

Komik *Database::getKomikById(int id)
{
    if (!db)
        return nullptr;

    const char *sql = "SELECT id, title, author, genre FROM komiks WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    Komik *komik = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string genre = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        komik = new Komik(id, title, author, genre);
    }

    sqlite3_finalize(stmt);
    return komik;
}

int Database::getLastKomikId()
{
    if (!db)
        return 0;

    const char *sql = "SELECT MAX(id) FROM komiks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int lastId = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return lastId;
}

// ===== GENRE OPERATIONS =====

bool Database::insertGenre(const string &name)
{
    if (!db)
        return false;

    const char *sql = "INSERT OR IGNORE INTO genres (name) VALUES (?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Database::updateGenre(const string &oldName, const string &newName)
{
    if (!db)
        return false;

    const char *sql = "UPDATE genres SET name = ? WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, oldName.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Database::deleteGenre(const string &name)
{
    if (!db)
        return false;

    const char *sql = "DELETE FROM genres WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

vector<string> Database::getAllGenres()
{
    vector<string> genres;
    if (!db)
        return genres;

    const char *sql = "SELECT name FROM genres ORDER BY name;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return genres;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        genres.push_back(name);
    }

    sqlite3_finalize(stmt);
    return genres;
}

bool Database::genreExists(const string &name)
{
    if (!db)
        return false;

    const char *sql = "SELECT COUNT(*) FROM genres WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

// ===== AUTHOR OPERATIONS =====

bool Database::insertAuthor(const string &name)
{
    if (!db)
        return false;

    const char *sql = "INSERT OR IGNORE INTO authors (name) VALUES (?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Database::updateAuthor(const string &oldName, const string &newName)
{
    if (!db)
        return false;

    const char *sql = "UPDATE authors SET name = ? WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, oldName.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Database::deleteAuthor(const string &name)
{
    if (!db)
        return false;

    const char *sql = "DELETE FROM authors WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

vector<string> Database::getAllAuthors()
{
    vector<string> authors;
    if (!db)
        return authors;

    const char *sql = "SELECT name FROM authors ORDER BY name;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return authors;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        authors.push_back(name);
    }

    sqlite3_finalize(stmt);
    return authors;
}

bool Database::authorExists(const string &name)
{
    if (!db)
        return false;

    const char *sql = "SELECT COUNT(*) FROM authors WHERE name = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

// ===== USER OPERATIONS =====

bool Database::insertUser(const User &user)
{
    if (!db)
        return false;

    const char *sql = "INSERT INTO users (id, username, password, role) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, user.id);
    sqlite3_bind_text(stmt, 2, user.username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.role.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        cerr << "Failed to insert user: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    cout << "User inserted to database: " << user.username << endl;
    return true;
}

bool Database::updateUser(const User &user)
{
    if (!db)
        return false;

    const char *sql = "UPDATE users SET password = ?, role = ? WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.role.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool Database::deleteUser(const string &username)
{
    if (!db)
        return false;

    const char *sql = "DELETE FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

vector<User> Database::getAllUsers()
{
    vector<User> users;
    if (!db)
        return users;

    const char *sql = "SELECT id, username, password, role FROM users;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        string username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string role = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        User user(id, username, password, role);
        users.push_back(user);
    }

    sqlite3_finalize(stmt);
    return users;
}

User *Database::getUserByUsername(const string &username)
{
    if (!db)
        return nullptr;

    const char *sql = "SELECT id, username, password, role FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    User *user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        string uname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string pass = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string role = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        user = new User(id, uname, pass, role);
    }

    sqlite3_finalize(stmt);
    return user;
}

bool Database::userExists(const string &username)
{
    if (!db)
        return false;

    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

int Database::getLastUserId()
{
    if (!db)
        return 0;

    const char *sql = "SELECT MAX(id) FROM users;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int lastId = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return lastId;
}

// ===== FAVORITES OPERATIONS =====

bool Database::addFavorite(const string &username, int komikId)
{
    if (!db)
        return false;

    const char *sql = "INSERT OR IGNORE INTO favorites (username, komik_id) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, komikId);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        cerr << "Failed to add favorite: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    return true;
}

bool Database::removeFavorite(const string &username, int komikId)
{
    if (!db)
        return false;

    const char *sql = "DELETE FROM favorites WHERE username = ? AND komik_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, komikId);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

vector<int> Database::getFavoritesByUser(const string &username)
{
    vector<int> favorites;
    if (!db)
        return favorites;

    const char *sql = "SELECT komik_id FROM favorites WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return favorites;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int komikId = sqlite3_column_int(stmt, 0);
        favorites.push_back(komikId);
    }

    sqlite3_finalize(stmt);
    return favorites;
}

bool Database::isFavorite(const string &username, int komikId)
{
    if (!db)
        return false;

    const char *sql = "SELECT COUNT(*) FROM favorites WHERE username = ? AND komik_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, komikId);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}