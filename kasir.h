#ifndef KASIR_H
#define KASIR_H

#include <iostream>
#include <string>
using namespace std;

struct menu { // ini struct buat menu produknya 
    string namaProduk;
    string kategoriProduk;
    int harga;
    int stok;
};

struct kasir{ // ini struct buat node di BST nya
    Data menu;
    kasir* kiri; 
    kasir* kanan;
};

struct item{ // kalau yang ini struct buat item/menu yang di taruh ke keranjang
    string namaMenu;
    int jumlah;
    int hargaSatuan;
    int totalHarga;
    item* next;
};

kasir* buatNode(menu dataMenu); // enih buat bikin node baru di BST nya
kasir* tambahMenu(kasir* daftarMenu, string nama, string kategori, int harga, int stok); // ini buat nambahin menu baru ke BST
void tampilkanMenuPreOrder(kasir* daftarMenu); // ini buat nampilin menu nya dengan cara pre order
void tampilkanMenuInOrder(kasir* daftarMenu); // ini buat nampilin menu nya dengan cara in order
void tampilkanMenuPostOrder(kasir* daftarMenu); // ini buat nampilin menu nya dengan cara post order
kasir* cariMenu(kasir* daftarMenu, string nama); // ini buat nyari menu berdasarkan nama
kasir* hapusMenu(kasir* daftarMenu, string nama); // ini buat hapus menu berdasarkan nama
kasir* cariMenuTerkecil(kasir* daftarMenu); // ini buat nyari menu dengan harga terkecil
void updateStokMenu(kasir* daftarMenu, string nama, int stokBaru); // ini buat update stok menu
void tambahKeKeranjang(item*& keranjang, string namaMenu, int jumlah, int hargaSatuan); // ini buat nambahin item ke keranjang
void tampilkanKeranjang(item* keranjang); // ini buat nampilin isi keranjang
int hitungTotalBayar(item* keranjang);  // ini buat ngitung total bayar dari isi keranjang
void cetakStruk(item* keranjang, int totalBayar); // ini buat cetak struk pembayaran
void bersihkanKeranjang(item*& keranjang); // ini buat bersihin isi keranjang
boolean login(); // ini tu buat bikin fungsi login kasir nya

#endif