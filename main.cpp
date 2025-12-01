#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    if (!connectDatabase("kasir.db")) {
        cerr << "Gagal terhubung ke database. Program dihentikan." << endl;
        return 1;
    }
    
    if (!login()) {
        disconnectDatabase();
        return 1;
    }
    
    Node* root = nullptr;
    vector<Menu> daftarMenu = ambilSemuaMenu();
    
    for (const auto& menu : daftarMenu) {
        root = tambahMenu(root, menu.nama, menu.kategori, menu.harga, menu.stok);
    }
    
    menuUtama();
    
    hapusSemuaNode(root);
    disconnectDatabase();
    
    return 0;
}