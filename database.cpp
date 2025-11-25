#include "kasir.h"
#include <libpq-fe.h>
#include <iostream>

using namespace std;

// Global connection variable
static PGconn* conn = nullptr;

PGconn* getConnection() {
    return conn;
}

bool connectDatabase(
    const string& host,
    const string& user,
    const string& password,
    const string& dbname,
    int port
) {
    string connInfo = "host=" + host + 
                      " port=" + to_string(port) +
                      " user=" + user + 
                      " password=" + password + 
                      " dbname=" + dbname;
    
    conn = PQconnectdb(connInfo.c_str());
    
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Koneksi database gagal: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        conn = nullptr;
        return false;
    }
    
    cout << "Koneksi database berhasil!" << endl;
    return true;
}

bool disconnectDatabase() {
    if (conn != nullptr) {
        PQfinish(conn);
        conn = nullptr;
        cout << "Database terputus." << endl;
        return true;
    }
    return false;
}

bool isConnected() {
    return (conn != nullptr && PQstatus(conn) == CONNECTION_OK);
}

bool simpanMenuDatabase(const Menu& data) {
    if (!isConnected()) {
        cerr << "Tidak terhubung ke database!" << endl;
        return false;
    }
    
    string query = "INSERT INTO menu (nama, kategori, harga, stok) VALUES ('" 
                   + data.nama + "', '" 
                   + data.kategori + "', " 
                   + to_string(data.harga) + ", " 
                   + to_string(data.stok) + ")";
    
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error menyimpan menu: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    
    PQclear(res);
    return true;
}

vector<Menu> ambilSemuaMenu() {
    vector<Menu> daftarMenu;
    
    if (!isConnected()) {
        cerr << "Tidak terhubung ke database!" << endl;
        return daftarMenu;
    }
    
    string query = "SELECT nama, kategori, harga, stok FROM menu";
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error mengambil menu: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return daftarMenu;
    }
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        Menu m;
        m.nama = PQgetvalue(res, i, 0);
        m.kategori = PQgetvalue(res, i, 1);
        m.harga = atoi(PQgetvalue(res, i, 2));
        m.stok = atoi(PQgetvalue(res, i, 3));
        daftarMenu.push_back(m);
    }
    
    PQclear(res);
    return daftarMenu;
}

bool updateMenuDatabase(const Menu& data) {
    if (!isConnected()) {
        cerr << "Tidak terhubung ke database!" << endl;
        return false;
    }
    
    string query = "UPDATE menu SET kategori='" + data.kategori + 
                   "', harga=" + to_string(data.harga) + 
                   ", stok=" + to_string(data.stok) + 
                   " WHERE nama='" + data.nama + "'";
    
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error update menu: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    
    PQclear(res);
    return true;
}

bool hapusMenuDatabase(const string& nama) {
    if (!isConnected()) {
        cerr << "Tidak terhubung ke database!" << endl;
        return false;
    }
    
    string query = "DELETE FROM menu WHERE nama='" + nama + "'";
    
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error hapus menu: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    
    PQclear(res);
    return true;
}

bool cekUserLogin(const string& username, const string& password) {
    if (!isConnected()) {
        cerr << "Tidak terhubung ke database!" << endl;
        return false;
    }
    
    string query = "SELECT * FROM users WHERE username='" + username + 
                   "' AND password='" + password + "'";
    
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error login: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    
    bool isValid = (PQntuples(res) > 0);
    PQclear(res);
    
    return isValid;
}