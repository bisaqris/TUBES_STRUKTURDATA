#ifndef KASIR_H
#define KASIR_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Forward declaration untuk PostgreSQL
struct pg_conn;
typedef struct pg_conn PGconn;

// =====================================
// Data Structure
// =====================================

struct Menu {
    string nama;
    string kategori;
    int harga;
    int stok;
};

struct Node {
    Menu data;
    Node* kiri;
    Node* kanan;
};

struct Item {
    string nama;
    int jumlah;
    int hargaSatuan;
    int totalHarga;
    Item* next;
};

// =====================================
// Database Functions
// =====================================

bool connectDatabase(
    const string& host,
    const string& user,
    const string& password,
    const string& dbname,
    int port = 5432
);

bool disconnectDatabase();
bool isConnected();
PGconn* getConnection();

bool simpanMenuDatabase(const Menu& data);
vector<Menu> ambilSemuaMenu(); 
bool updateMenuDatabase(const Menu& data);
bool hapusMenuDatabase(const string& nama);
bool cekUserLogin(const string& username, const string& password);

// =====================================
// Binary Search Tree Functions
// =====================================

Node* buatNode(const Menu& data);

Node* tambahMenu(
    Node* root, 
    const string& nama, 
    const string& kategori, 
    int harga, 
    int stok
);

void tampilkanPreOrder(Node* root);
void tampilkanInOrder(Node* root);
void tampilkanPostOrder(Node* root);

Node* cariMenu(Node* root, const string& nama);
Node* hapusMenu(Node* root, const string& nama);
Node* cariMenuTerkecil(Node* root);
bool updateStokMenu(Node* root, const string& nama, int stokBaru);
void hapusSemuaNode(Node* root);

// =====================================
// Shopping Cart Functions
// =====================================

void tambahKeKeranjang(
    Item*& keranjang, 
    const string& namaMenu, 
    int jumlah, 
    int hargaSatuan
);

void tampilkanKeranjang(Item* keranjang);
int hitungTotalBayar(Item* keranjang);
void cetakStruk(Item* keranjang, int totalBayar);
void bersihkanKeranjang(Item*& keranjang);

// =====================================
// Auth & UI Functions
// =====================================

bool login();
void clearScreen();
void pause();

#endif