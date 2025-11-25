#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

void tambahKeKeranjang(
    Item*& keranjang, 
    const string& namaMenu, 
    int jumlah, 
    int hargaSatuan
) {
    Item* itemBaru = new Item;
    itemBaru->nama = namaMenu;
    itemBaru->jumlah = jumlah;
    itemBaru->hargaSatuan = hargaSatuan;
    itemBaru->totalHarga = jumlah * hargaSatuan;
    itemBaru->next = nullptr;
    
    if (keranjang == nullptr) {
        keranjang = itemBaru;
    } else {
        // Cek apakah item sudah ada
        Item* temp = keranjang;
        Item* prev = nullptr;
        
        while (temp != nullptr) {
            if (temp->nama == namaMenu) {
                // Update jumlah jika sudah ada
                temp->jumlah += jumlah;
                temp->totalHarga = temp->jumlah * temp->hargaSatuan;
                delete itemBaru;
                cout << "Jumlah item diperbarui!" << endl;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        
        // Tambah di akhir jika belum ada
        prev->next = itemBaru;
    }
    
    cout << "Item berhasil ditambahkan ke keranjang!" << endl;
}

void tampilkanKeranjang(Item* keranjang) {
    if (keranjang == nullptr) {
        cout << "Keranjang kosong!" << endl;
        return;
    }
    
    cout << "\n===============================================" << endl;
    cout << "              KERANJANG BELANJA               " << endl;
    cout << "===============================================" << endl;
    cout << left << setw(20) << "Nama Item" 
         << setw(10) << "Jumlah" 
         << setw(15) << "Harga Satuan" 
         << "Total" << endl;
    cout << "-----------------------------------------------" << endl;
    
    Item* temp = keranjang;
    while (temp != nullptr) {
        cout << left << setw(20) << temp->nama 
             << setw(10) << temp->jumlah 
             << "Rp " << setw(12) << temp->hargaSatuan 
             << "Rp " << temp->totalHarga << endl;
        temp = temp->next;
    }
    
    cout << "===============================================" << endl;
}

int hitungTotalBayar(Item* keranjang) {
    int total = 0;
    Item* temp = keranjang;
    
    while (temp != nullptr) {
        total += temp->totalHarga;
        temp = temp->next;
    }
    
    return total;
}

void cetakStruk(Item* keranjang, int totalBayar) {
    cout << "\n===============================================" << endl;
    cout << "                  STRUK BELANJA                " << endl;
    cout << "            SISTEM KASIR RESTORAN              " << endl;
    cout << "===============================================" << endl;
    
    Item* temp = keranjang;
    while (temp != nullptr) {
        cout << left << setw(20) << temp->nama 
             << setw(5) << temp->jumlah << " x " 
             << setw(10) << temp->hargaSatuan 
             << " = Rp " << temp->totalHarga << endl;
        temp = temp->next;
    }
    
    cout << "-----------------------------------------------" << endl;
    cout << right << setw(35) << "Total: Rp " << totalBayar << endl;
    cout << "===============================================" << endl;
    cout << "       Terima kasih atas kunjungan Anda!      " << endl;
    cout << "===============================================\n" << endl;
}

void bersihkanKeranjang(Item*& keranjang) {
    while (keranjang != nullptr) {
        Item* temp = keranjang;
        keranjang = keranjang->next;
        delete temp;
    }
    cout << "Keranjang berhasil dikosongkan!" << endl;
}