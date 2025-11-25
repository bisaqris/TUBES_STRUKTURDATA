#ifndef KASIR_H
#define KASIR_H

#include <iostream>
#include <string>
using namespace std;

struct menu { 
    string namaProduk;
    string kategoriProduk;
    int harga;
    int stok;
};

struct kasir{
    menu dataMenu;
    kasir* kiri; 
    kasir* kanan;
};

struct item{ 
    string namaMenu;
    int jumlah;
    int hargaSatuan;
    int totalHarga;
    item* next;
};

kasir* buatNode(menu dataMenu);

kasir* tambahMenu(kasir* daftarMenu, string nama, string kategori, int harga, int stok); 
void tampilkanMenu(kasir* daftarMenu); 
kasir* cariMenu(kasir* daftarMenu, string nama); 
kasir* hapusMenu(kasir* daftarMenu, string nama);
void updateStokMenu(kasir* daftarMenu, string nama, int stokBaru); 

kasir* findMin(kasir* root);

int hitungTotalBayar(item* keranjang);  
void printStruk(item* keranjang, int totalBayar); 

bool login(); 

#endif