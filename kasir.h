#ifndef KASIR_H
#define KASIR_H

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

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

bool connectDatabase(const string& dbPath = "kasir.db");
bool disconnectDatabase();
bool isConnected();
sqlite3* getConnection();
bool initializeDatabase();

bool simpanMenuDatabase(const Menu& data);
vector<Menu> ambilSemuaMenu(); 
bool updateMenuDatabase(const Menu& data);
bool hapusMenuDatabase(const string& nama);
bool cekUserLogin(const string& username, const string& password);

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

void tambahKeKeranjang(
    Item*& keranjang, 
    const string& namaMenu, 
    int jumlah, 
    int hargaSatuan
);

void tambahKeKeranjang(Item*& keranjang, const string& namaMenu, int jumlah, int hargaSatuan);
void tampilkanKeranjang(Item* keranjang);
int hitungTotalBayar(Item* keranjang);
void cetakStruk(Item* keranjang, int totalBayar);
void bersihkanKeranjang(Item*& keranjang);

void menuUtama();
void menuAdmin(Node*& root);
void menuKasir(Node* root);

bool login();
void clearScreen();
void pause();

#endif