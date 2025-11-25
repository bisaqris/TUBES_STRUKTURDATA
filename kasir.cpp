#include "kasir.h"

kasir* buatNode(menu dataMenu) {
    kasir* baru = new kasir;
    baru->dataMenu = dataMenu;
    baru->kiri = baru->kanan = nullptr;
    return baru;
}

kasir* tambahMenu(kasir* daftarMenu, string nama, string kategori, int harga, int stok) {
    if (!daftarMenu) {
        menu m; m.namaProduk = nama; m.kategoriProduk = kategori; m.harga = harga; m.stok = stok;
        return buatNode(m);
    }
    if (nama < daftarMenu->dataMenu.namaProduk)
        daftarMenu->kiri = tambahMenu(daftarMenu->kiri, nama, kategori, harga, stok);
    else
        daftarMenu->kanan = tambahMenu(daftarMenu->kanan, nama, kategori, harga, stok);

    return daftarMenu;
}

void tampilkanMenu(kasir* daftarMenu) {
    if (!daftarMenu) return;
    tampilkanMenu(daftarMenu->kiri);
    cout << daftarMenu->dataMenu.namaProduk << " | " 
         << daftarMenu->dataMenu.kategoriProduk << " | Rp "
         << daftarMenu->dataMenu.harga << " | Stok: "
         << daftarMenu->dataMenu.stok << endl;
    tampilkanMenu(daftarMenu->kanan);
}

kasir* cariMenu(kasir* daftarMenu, string nama) {
    if (!daftarMenu) return nullptr;
    if (daftarMenu->dataMenu.namaProduk == nama) return daftarMenu;
    if (nama < daftarMenu->dataMenu.namaProduk)
        return cariMenu(daftarMenu->kiri, nama);
    else
        return cariMenu(daftarMenu->kanan, nama);
}

kasir* findMin(kasir* root) {
    while (root && root->kiri)
        root = root->kiri;
    return root;
}

kasir* hapusMenu(kasir* daftarMenu, string nama) {
    if (!daftarMenu) return nullptr;

    if (nama < daftarMenu->dataMenu.namaProduk)
        daftarMenu->kiri = hapusMenu(daftarMenu->kiri, nama);
    else if (nama > daftarMenu->dataMenu.namaProduk)
        daftarMenu->kanan = hapusMenu(daftarMenu->kanan, nama);
    else {
        if (!daftarMenu->kiri && !daftarMenu->kanan) {
            delete daftarMenu;
            return nullptr;
        } else if (!daftarMenu->kiri) {
            kasir* temp = daftarMenu->kanan;
            delete daftarMenu;
            return temp;
        } else if (!daftarMenu->kanan) {
            kasir* temp = daftarMenu->kiri;
            delete daftarMenu;
            return temp;
        } else {
            kasir* temp = findMin(daftarMenu->kanan);
            daftarMenu->dataMenu = temp->dataMenu;
            daftarMenu->kanan = hapusMenu(daftarMenu->kanan, temp->dataMenu.namaProduk);
        }
    }
    return daftarMenu;
}

void updateStokMenu(kasir* daftarMenu, string nama, int stokBaru) {
    kasir* menuFound = cariMenu(daftarMenu, nama);
    if (menuFound)
        menuFound->dataMenu.stok = stokBaru;
}

int hitungTotalBayar(item* keranjang) {
    int total = 0;
    while (keranjang) {
        total += keranjang->totalHarga;
        keranjang = keranjang->next;
    }
    return total;
}

void printStruk(item* keranjang, int totalBayar) {
    cout << "\n===== STRUK PEMBELIAN =====\n";
    while (keranjang) {
        cout << keranjang->namaMenu << " x" << keranjang->jumlah
             << " = " << keranjang->totalHarga << endl;
        keranjang = keranjang->next;
    }
    cout << "TOTAL BAYAR: Rp " << totalBayar << endl;
    cout << "===========================\n";
}

bool login() {
    string username, password;
    cout << "=== LOGIN KASIR ===\n";
    cout << "Username : "; cin >> username;
    cout << "Password : "; cin >> password;
    return (username == "kasir" && password == "123");
}
