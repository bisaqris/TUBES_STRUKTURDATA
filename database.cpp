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
        "created_at DATETIME DEFAULT (datetime('now', '+7 hours'))"
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
        "created_at DATETIME DEFAULT (datetime('now', '+7 hours')),"
        "updated_at DATETIME DEFAULT (datetime('now', '+7 hours'))"
        ");";
    
    rc = sqlite3_exec(db, sqlMenu, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating menu table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Tabel Transaksi
    const char* sqlTransaksi = 
        "CREATE TABLE IF NOT EXISTS transaksi ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "tanggal DATETIME DEFAULT (datetime('now', '+7 hours')),"
        "total_bayar INTEGER NOT NULL,"
        "uang_diterima INTEGER NOT NULL,"
        "kembalian INTEGER NOT NULL"
        ");";
    
    rc = sqlite3_exec(db, sqlTransaksi, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating transaksi table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    // Tabel Detail Transaksi
    const char* sqlDetailTransaksi = 
        "CREATE TABLE IF NOT EXISTS detail_transaksi ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "transaksi_id INTEGER NOT NULL,"
        "nama_menu TEXT NOT NULL,"
        "jumlah INTEGER NOT NULL,"
        "harga_satuan INTEGER NOT NULL,"
        "subtotal INTEGER NOT NULL,"
        "FOREIGN KEY (transaksi_id) REFERENCES transaksi(id)"
        ");";
    
    rc = sqlite3_exec(db, sqlDetailTransaksi, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating detail_transaksi table: " << errMsg << endl;
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
    const char* sql = "UPDATE menu SET kategori=?, harga=?, stok=?, updated_at=datetime('now', '+7 hours') WHERE nama=?;";
    
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

// ========== FUNGSI TRANSAKSI ==========

bool simpanTransaksi(Item* keranjang, int totalBayar, int uangDiterima, int kembalian) {
    if (!isConnected() || keranjang == nullptr) {
        return false;
    }
    
    // Mulai transaksi database
    char* errMsg = nullptr;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg);
    
    // Simpan transaksi utama
    sqlite3_stmt* stmt;
    const char* sqlTransaksi = 
        "INSERT INTO transaksi (total_bayar, uang_diterima, kembalian) VALUES (?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sqlTransaksi, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, totalBayar);
    sqlite3_bind_int(stmt, 2, uangDiterima);
    sqlite3_bind_int(stmt, 3, kembalian);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    
    // Ambil ID transaksi yang baru saja dibuat
    int transaksiId = sqlite3_last_insert_rowid(db);
    
    // Simpan detail transaksi
    const char* sqlDetail = 
        "INSERT INTO detail_transaksi (transaksi_id, nama_menu, jumlah, harga_satuan, subtotal) "
        "VALUES (?, ?, ?, ?, ?);";
    
    Item* temp = keranjang;
    while (temp != nullptr) {
        rc = sqlite3_prepare_v2(db, sqlDetail, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        
        sqlite3_bind_int(stmt, 1, transaksiId);
        sqlite3_bind_text(stmt, 2, temp->nama.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, temp->jumlah);
        sqlite3_bind_int(stmt, 4, temp->hargaSatuan);
        sqlite3_bind_int(stmt, 5, temp->totalHarga);
        
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc != SQLITE_DONE) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        
        temp = temp->next;
    }
    
    // Commit transaksi
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;
}

vector<Transaksi> ambilSemuaTransaksi() {
    vector<Transaksi> daftarTransaksi;
    
    if (!isConnected()) {
        return daftarTransaksi;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = 
        "SELECT id, strftime('%Y-%m-%d %H:%M:%S', tanggal), total_bayar, uang_diterima, kembalian "
        "FROM transaksi ORDER BY tanggal DESC;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return daftarTransaksi;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaksi t;
        t.id = sqlite3_column_int(stmt, 0);
        t.tanggal = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        t.totalBayar = sqlite3_column_int(stmt, 2);
        t.uangDiterima = sqlite3_column_int(stmt, 3);
        t.kembalian = sqlite3_column_int(stmt, 4);
        daftarTransaksi.push_back(t);
    }
    
    sqlite3_finalize(stmt);
    return daftarTransaksi;
}

vector<DetailTransaksi> ambilDetailTransaksi(int transaksiId) {
    vector<DetailTransaksi> detail;
    
    if (!isConnected()) {
        return detail;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = 
        "SELECT nama_menu, jumlah, harga_satuan, subtotal "
        "FROM detail_transaksi WHERE transaksi_id = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return detail;
    }
    
    sqlite3_bind_int(stmt, 1, transaksiId);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        DetailTransaksi d;
        d.namaMenu = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        d.jumlah = sqlite3_column_int(stmt, 1);
        d.hargaSatuan = sqlite3_column_int(stmt, 2);
        d.subtotal = sqlite3_column_int(stmt, 3);
        detail.push_back(d);
    }
    
    sqlite3_finalize(stmt);
    return detail;
}

int hitungTotalPenjualanHariIni() {
    if (!isConnected()) {
        return 0;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = 
        "SELECT COALESCE(SUM(total_bayar), 0) FROM transaksi "
        "WHERE DATE(tanggal) = DATE(datetime('now', '+7 hours'));";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return total;
}

int hitungTotalPenjualanBulanIni() {
    if (!isConnected()) {
        return 0;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = 
        "SELECT COALESCE(SUM(total_bayar), 0) FROM transaksi "
        "WHERE strftime('%Y-%m', tanggal) = strftime('%Y-%m', datetime('now', '+7 hours'));";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    int total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return total;
}

vector<pair<string, int>> getMenuTerlaris(int limit) {
    vector<pair<string, int>> menuTerlaris;
    
    if (!isConnected()) {
        return menuTerlaris;
    }
    
    sqlite3_stmt* stmt;
    const char* sql = 
        "SELECT nama_menu, SUM(jumlah) as total_terjual "
        "FROM detail_transaksi "
        "GROUP BY nama_menu "
        "ORDER BY total_terjual DESC "
        "LIMIT ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return menuTerlaris;
    }
    
    sqlite3_bind_int(stmt, 1, limit);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string nama = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int jumlah = sqlite3_column_int(stmt, 1);
        menuTerlaris.push_back(make_pair(nama, jumlah));
    }
    
    sqlite3_finalize(stmt);
    return menuTerlaris;
}