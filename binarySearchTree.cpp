#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

Node* buatNode(const Menu& data) {
    Node* nodeBaru = new Node;
    nodeBaru->data = data;
    nodeBaru->kiri = nullptr;
    nodeBaru->kanan = nullptr;
    return nodeBaru;
}

Node* tambahMenu(Node* root, const string& nama, const string& kategori, 
                 int harga, int stok) {
    if (root == nullptr) {
        Menu m = {nama, kategori, harga, stok};
        return buatNode(m);
    }
    
    if (nama < root->data.nama) {
        root->kiri = tambahMenu(root->kiri, nama, kategori, harga, stok);
    } else if (nama > root->data.nama) {
        root->kanan = tambahMenu(root->kanan, nama, kategori, harga, stok);
    } else {
        cout << "Menu dengan nama '" << nama << "' sudah ada!" << endl;
    }
    
    return root;
}

void tampilkanInOrder(Node* root) {
    if (root != nullptr) {
        tampilkanInOrder(root->kiri);
        cout << left << setw(20) << root->data.nama 
             << setw(15) << root->data.kategori
             << "Rp " << setw(10) << root->data.harga
             << "Stok: " << root->data.stok << endl;
        tampilkanInOrder(root->kanan);
    }
}

Node* cariMenu(Node* root, const string& nama) {
    if (root == nullptr || root->data.nama == nama) {
        return root;
    }
    
    if (nama < root->data.nama) {
        return cariMenu(root->kiri, nama);
    }
    
    return cariMenu(root->kanan, nama);
}

Node* cariMenuTerkecil(Node* root) {
    while (root != nullptr && root->kiri != nullptr) {
        root = root->kiri;
    }
    return root;
}

Node* hapusMenu(Node* root, const string& nama) {
    if (root == nullptr) {
        return root;
    }
    
    if (nama < root->data.nama) {
        root->kiri = hapusMenu(root->kiri, nama);
    } else if (nama > root->data.nama) {
        root->kanan = hapusMenu(root->kanan, nama);
    } else {
        if (root->kiri == nullptr) {
            Node* temp = root->kanan;
            delete root;
            return temp;
        } else if (root->kanan == nullptr) {
            Node* temp = root->kiri;
            delete root;
            return temp;
        }
        
        Node* temp = cariMenuTerkecil(root->kanan);
        root->data = temp->data;
        root->kanan = hapusMenu(root->kanan, temp->data.nama);
    }
    
    return root;
}

bool updateStokMenu(Node* root, const string& nama, int stokBaru) {
    Node* node = cariMenu(root, nama);
    if (node != nullptr) {
        node->data.stok = stokBaru;
        return true;
    }
    return false;
}

void hapusSemuaNode(Node* root) {
    if (root != nullptr) {
        hapusSemuaNode(root->kiri);
        hapusSemuaNode(root->kanan);
        delete root;
    }
}

//POST-ORDER
NodeTransaksi* buatNodeTransaksi(const Transaksi& data) {
    NodeTransaksi* nodeBaru = new NodeTransaksi;
    nodeBaru->data = data;
    nodeBaru->kiri = nullptr;
    nodeBaru->kanan = nullptr;
    return nodeBaru;
}

NodeTransaksi* tambahTransaksi(NodeTransaksi* root, const Transaksi& data) {
    if (root == nullptr) {
        return buatNodeTransaksi(data);
    }
    if (data.id > root->data.id) {
        root->kiri = tambahTransaksi(root->kiri, data);
    } else if (data.id < root->data.id) {
        root->kanan = tambahTransaksi(root->kanan, data);
    }
    
    return root;
}

void tampilkanTransaksiPostOrder(NodeTransaksi* root) {
    if (root != nullptr) {
        tampilkanTransaksiPostOrder(root->kiri);
        tampilkanTransaksiPostOrder(root->kanan);

        cout << left << setw(5) << root->data.id
             << setw(22) << root->data.tanggal
             << "Rp " << setw(12) << root->data.totalBayar
             << "Rp " << setw(12) << root->data.uangDiterima
             << "Rp " << root->data.kembalian << endl;
    }
}

void hapusSemuaNodeTransaksi(NodeTransaksi* root) {
    if (root != nullptr) {
        hapusSemuaNodeTransaksi(root->kiri);
        hapusSemuaNodeTransaksi(root->kanan);
        delete root;
    }
}

//postorder
NodeMenuTerlaris* buatNodeMenuTerlaris(const string& nama, int jumlah) {
    NodeMenuTerlaris* nodeBaru = new NodeMenuTerlaris;
    nodeBaru->namaMenu = nama;
    nodeBaru->jumlahTerjual = jumlah;
    nodeBaru->kiri = nullptr;
    nodeBaru->kanan = nullptr;
    return nodeBaru;
}

NodeMenuTerlaris* tambahMenuTerlaris(NodeMenuTerlaris* root, const string& nama, int jumlah) {
    if (root == nullptr) {
        return buatNodeMenuTerlaris(nama, jumlah);
    }
    if (jumlah > root->jumlahTerjual) {
        root->kiri = tambahMenuTerlaris(root->kiri, nama, jumlah);
    } else if (jumlah < root->jumlahTerjual) {
        root->kanan = tambahMenuTerlaris(root->kanan, nama, jumlah);
    } else {
        if (nama < root->namaMenu) {
            root->kiri = tambahMenuTerlaris(root->kiri, nama, jumlah);
        } else {
            root->kanan = tambahMenuTerlaris(root->kanan, nama, jumlah);
        }
    }
    
    return root;
}

void tampilkanMenuTerlarisPostOrder(NodeMenuTerlaris* root, int& nomor) {
    if (root != nullptr) {
        tampilkanMenuTerlarisPostOrder(root->kiri, nomor);
        tampilkanMenuTerlarisPostOrder(root->kanan, nomor);
        
        cout << left << setw(5) << nomor++
             << setw(25) << root->namaMenu
             << root->jumlahTerjual << " porsi" << endl;
    }
}

void tampilkanMenuTerlarisPreOrder(NodeMenuTerlaris* root, int& nomor) {
    if (root != nullptr) {
        cout << left << setw(5) << nomor++
             << setw(25) << root->namaMenu
             << root->jumlahTerjual << " porsi" << endl;
        
        tampilkanMenuTerlarisPreOrder(root->kiri, nomor);
        tampilkanMenuTerlarisPreOrder(root->kanan, nomor);
    }
}

void hapusSemuaNodeMenuTerlaris(NodeMenuTerlaris* root) {
    if (root != nullptr) {
        hapusSemuaNodeMenuTerlaris(root->kiri);
        hapusSemuaNodeMenuTerlaris(root->kanan);
        delete root;
    }
}