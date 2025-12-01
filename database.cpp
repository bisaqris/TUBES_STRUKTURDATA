#include "kasir.h"
#include <iostream>
#include <sqlite3.h>

using namespace std;

static sqlite3* db = nullptr;

bool connectDatabase(const string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        cerr << "Error: Tidak dapat membuka database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        db = nullptr;
        return false;
    }
    
    cout << "Koneksi database berhasil!" << endl;
    
    return initializeDatabase();
}

bool disconnectDatabase() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database terputus." << endl;
        return true;
    }
    return false;
}

bool isConnected() {
    return db != nullptr;
}

sqlite3* getConnection() {
    return db;
}

bool initializeDatabase() {
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return false;
    }
    
    char* errMsg = nullptr;
    
    const char* sqlUsers = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "role TEXT DEFAULT 'kasir',"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    int rc = sqlite3_exec(db, sqlUsers, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating users table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    const char* sqlMenu = 
        "CREATE TABLE IF NOT EXISTS menu ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nama TEXT NOT NULL UNIQUE,"
        "kategori TEXT NOT NULL,"
        "harga INTEGER NOT NULL,"
        "stok INTEGER NOT NULL DEFAULT 0,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    rc = sqlite3_exec(db, sqlMenu, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating menu table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    const char* sqlInsertUsers = 
        "INSERT OR IGNORE INTO users (username, password, role) VALUES "
        "('kasir', 'kasir123', 'kasir');";
    
    rc = sqlite3_exec(db, sqlInsertUsers, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error inserting default users: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    const char* sqlInsertMenu = 
        "INSERT OR IGNORE INTO menu (nama, kategori, harga, stok) VALUES "
        "('Nasi Goreng', 'Makanan', 15000, 50),"
        "('Mie Goreng', 'Makanan', 12000, 30),"
        "('Ayam Bakar', 'Makanan', 25000, 20),"
        "('Soto Ayam', 'Makanan', 18000, 25),"
        "('Teh Manis', 'Minuman', 5000, 100),"
        "('Jeruk Peras', 'Minuman', 8000, 50),"
        "('Es Teh', 'Minuman', 5000, 100),"
        "('Kopi Hitam', 'Minuman', 7000, 75);";
    
    rc = sqlite3_exec(db, sqlInsertMenu, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error inserting default menu: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool simpanMenuDatabase(const Menu& data) {
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return false;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO menu (nama, kategori, harga, stok) VALUES (?, ?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, data.nama.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, data.kategori.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, data.harga);
    sqlite3_bind_int(stmt, 4, data.stok);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << "Error: Gagal menyimpan menu: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    return true;
}

vector<Menu> ambilSemuaMenu() {
    vector<Menu> daftarMenu;
    
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return daftarMenu;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT nama, kategori, harga, stok FROM menu;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return daftarMenu;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Menu m;
        m.nama = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        m.kategori = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        m.harga = sqlite3_column_int(stmt, 2);
        m.stok = sqlite3_column_int(stmt, 3);
        daftarMenu.push_back(m);
    }
    
    sqlite3_finalize(stmt);
    return daftarMenu;
}

bool updateMenuDatabase(const Menu& data) {
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return false;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE menu SET kategori=?, harga=?, stok=?, updated_at=CURRENT_TIMESTAMP WHERE nama=?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, data.kategori.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, data.harga);
    sqlite3_bind_int(stmt, 3, data.stok);
    sqlite3_bind_text(stmt, 4, data.nama.c_str(), -1, SQLITE_TRANSIENT);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << "Error: Gagal update menu: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    return sqlite3_changes(db) > 0;
}

bool hapusMenuDatabase(const string& nama) {
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return false;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM menu WHERE nama=?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, nama.c_str(), -1, SQLITE_TRANSIENT);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << "Error: Gagal hapus menu: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    return sqlite3_changes(db) > 0;
}

bool cekUserLogin(const string& username, const string& password) {
    if (!isConnected()) {
        cerr << "Error: Tidak terhubung ke database" << endl;
        return false;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM users WHERE username=? AND password=?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    
    rc = sqlite3_step(stmt);
    bool found = (rc == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    return found;
}