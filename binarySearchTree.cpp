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

void tampilkanPreOrder(Node* root) {
    if (root != nullptr) {
        cout << left << setw(20) << root->data.nama 
             << setw(15) << root->data.kategori
             << "Rp " << setw(10) << root->data.harga
             << "Stok: " << root->data.stok << endl;
        tampilkanPreOrder(root->kiri);
        tampilkanPreOrder(root->kanan);
    }
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

void tampilkanPostOrder(Node* root) {
    if (root != nullptr) {
        tampilkanPostOrder(root->kiri);
        tampilkanPostOrder(root->kanan);
        cout << left << setw(20) << root->data.nama 
             << setw(15) << root->data.kategori
             << "Rp " << setw(10) << root->data.harga
             << "Stok: " << root->data.stok << endl;
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