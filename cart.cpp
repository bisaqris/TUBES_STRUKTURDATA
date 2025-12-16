#include "kasir.h"
#include <iostream>
#include <iomanip>
using namespace std;

void tambahKeKeranjang(Item*& keranjang, const string& namaMenu, int jumlah, int hargaSatuan) {
    Item* itemBaru = new Item;
    itemBaru->nama = namaMenu;
    itemBaru->jumlah = jumlah;
    itemBaru->hargaSatuan = hargaSatuan;
    itemBaru->totalHarga = jumlah * hargaSatuan;
    itemBaru->next = nullptr;
    
    if (keranjang == nullptr) {
        keranjang = itemBaru;
    } else {
        Item* temp = keranjang;
        while (temp != nullptr) {
            if (temp->nama == namaMenu) {
                temp->jumlah += jumlah;
                temp->totalHarga = temp->jumlah * temp->hargaSatuan;
                delete itemBaru;
                return;
            }
            if (temp->next == nullptr) break;
            temp = temp->next;
        }
        temp->next = itemBaru;
    }
}

void tampilkanKeranjang(Item *keranjang)
{
    if (keranjang == nullptr)
    {
        cout << "Keranjang kosong!" << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "           DAFTAR BELANJA" << endl;
    cout << "========================================" << endl;
    cout << left << setw(20) << "Nama Item"
         << setw(10) << "Jumlah"
         << setw(15) << "Harga Satuan"
         << "Total" << endl;
    cout << "----------------------------------------" << endl;

    Item *temp = keranjang;
    while (temp != nullptr)
    {
        cout << left << setw(20) << temp->nama
             << setw(10) << temp->jumlah
             << "Rp " << setw(12) << temp->hargaSatuan
             << "Rp " << temp->totalHarga << endl;
        temp = temp->next;
    }
    cout << "========================================" << endl;
}

int hitungTotalBayar(Item *keranjang)
{
    int total = 0;
    Item *temp = keranjang;
    while (temp != nullptr)
    {
        total += temp->totalHarga;
        temp = temp->next;
    }
    return total;
}

void cetakStruk(Item *keranjang, int totalBayar)
{
    cout << "\n========================================" << endl;
    cout << "              STRUK PEMBELIAN" << endl;
    cout << "========================================" << endl;
    cout << left << setw(20) << "Item"
         << setw(10) << "Qty"
         << setw(15) << "Harga"
         << "Subtotal" << endl;
    cout << "----------------------------------------" << endl;

    Item *temp = keranjang;
    while (temp != nullptr)
    {
        cout << left << setw(20) << temp->nama
             << setw(10) << temp->jumlah
             << "Rp " << setw(12) << temp->hargaSatuan
             << "Rp " << temp->totalHarga << endl;
        temp = temp->next;
    }

    cout << "========================================" << endl;
    cout << "TOTAL BAYAR: Rp " << totalBayar << endl;
    cout << "========================================" << endl;
    cout << "      Terima kasih atas pembelian Anda!" << endl;
    cout << "========================================\n" << endl;
}

void bersihkanKeranjang(Item *&keranjang)
{
    Item *temp;
    while (keranjang != nullptr)
    {
        temp = keranjang;
        keranjang = keranjang->next;
        delete temp;
    }
}
