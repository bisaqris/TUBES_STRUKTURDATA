#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

void tampilkanMenuUtama() {
    cout << "\n===============================================" << endl;
    cout << "         SISTEM KASIR RESTORAN                 " << endl;
    cout << "===============================================" << endl;
    cout << "1. Kelola Menu" << endl;
    cout << "2. Transaksi Penjualan" << endl;
    cout << "3. Keluar" << endl;
    cout << "===============================================" << endl;
    cout << "Pilih menu: ";
}

void menuKelolaMenu(Node*& root) {
    int pilihan;
    
    do {
        clearScreen();
        cout << "\n===============================================" << endl;
        cout << "              KELOLA MENU                      " << endl;
        cout << "===============================================" << endl;
        cout << "1. Tambah Menu" << endl;
        cout << "2. Tampilkan Menu (In-Order)" << endl;
        cout << "3. Tampilkan Menu (Pre-Order)" << endl;
        cout << "4. Tampilkan Menu (Post-Order)" << endl;
        cout << "5. Cari Menu" << endl;
        cout << "6. Update Stok Menu" << endl;
        cout << "7. Hapus Menu" << endl;
        cout << "8. Kembali" << endl;
        cout << "===============================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        
        string nama, kategori;
        int harga, stok;
        
        switch (pilihan) {
            case 1:
                cout << "\n--- Tambah Menu Baru ---" << endl;
                cin.ignore();
                cout << "Nama Menu: ";
                getline(cin, nama);
                cout << "Kategori: ";
                getline(cin, kategori);
                cout << "Harga: ";
                cin >> harga;
                cout << "Stok: ";
                cin >> stok;
                
                root = tambahMenu(root, nama, kategori, harga, stok);
                pause();
                break;
                
            case 2:
                cout << "\n--- Daftar Menu (In-Order) ---" << endl;
                if (root == nullptr) {
                    cout << "Belum ada menu!" << endl;
                } else {
                    tampilkanInOrder(root);
                }
                pause();
                break;
                
            case 3:
                cout << "\n--- Daftar Menu (Pre-Order) ---" << endl;
                if (root == nullptr) {
                    cout << "Belum ada menu!" << endl;
                } else {
                    tampilkanPreOrder(root);
                }
                pause();
                break;
                
            case 4:
                cout << "\n--- Daftar Menu (Post-Order) ---" << endl;
                if (root == nullptr) {
                    cout << "Belum ada menu!" << endl;
                } else {
                    tampilkanPostOrder(root);
                }
                pause();
                break;
                
            case 5:
                cin.ignore();
                cout << "\nNama menu yang dicari: ";
                getline(cin, nama);
                {
                    Node* hasil = cariMenu(root, nama);
                    if (hasil != nullptr) {
                        cout << "\nMenu ditemukan!" << endl;
                        cout << "Nama: " << hasil->data.nama << endl;
                        cout << "Kategori: " << hasil->data.kategori << endl;
                        cout << "Harga: Rp " << hasil->data.harga << endl;
                        cout << "Stok: " << hasil->data.stok << endl;
                    } else {
                        cout << "Menu tidak ditemukan!" << endl;
                    }
                }
                pause();
                break;
                
            case 6:
                cin.ignore();
                cout << "\nNama menu: ";
                getline(cin, nama);
                cout << "Stok baru: ";
                cin >> stok;
                
                if (updateStokMenu(root, nama, stok)) {
                    cout << "Stok berhasil diupdate!" << endl;
                } else {
                    cout << "Menu tidak ditemukan!" << endl;
                }
                pause();
                break;
                
            case 7:
                cin.ignore();
                cout << "\nNama menu yang akan dihapus: ";
                getline(cin, nama);
                root = hapusMenu(root, nama);
                pause();
                break;
                
            case 8:
                cout << "Kembali ke menu utama..." << endl;
                break;
                
            default:
                cout << "Pilihan tidak valid!" << endl;
                pause();
        }
        
    } while (pilihan != 8);
}

void menuTransaksi(Node* root) {
    Item* keranjang = nullptr;
    int pilihan;
    
    do {
        clearScreen();
        cout << "\n===============================================" << endl;
        cout << "            TRANSAKSI PENJUALAN                " << endl;
        cout << "===============================================" << endl;
        cout << "1. Tampilkan Menu" << endl;
        cout << "2. Tambah Item ke Keranjang" << endl;
        cout << "3. Lihat Keranjang" << endl;
        cout << "4. Proses Pembayaran" << endl;
        cout << "5. Batalkan Transaksi" << endl;
        cout << "6. Kembali" << endl;
        cout << "===============================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        
        string nama;
        int jumlah;
        
        switch (pilihan) {
            case 1:
                cout << "\n--- Daftar Menu ---" << endl;
                if (root == nullptr) {
                    cout << "Belum ada menu!" << endl;
                } else {
                    tampilkanInOrder(root);
                }
                pause();
                break;
                
            case 2:
                cin.ignore();
                cout << "\nNama menu: ";
                getline(cin, nama);
                cout << "Jumlah: ";
                cin >> jumlah;
                
                {
                    Node* menu = cariMenu(root, nama);
                    if (menu != nullptr) {
                        if (menu->data.stok >= jumlah) {
                            tambahKeKeranjang(keranjang, nama, jumlah, menu->data.harga);
                            // Update stok sementara (akan disimpan saat checkout)
                            menu->data.stok -= jumlah;
                        } else {
                            cout << "Stok tidak mencukupi! Stok tersedia: " << menu->data.stok << endl;
                        }
                    } else {
                        cout << "Menu tidak ditemukan!" << endl;
                    }
                }
                pause();
                break;
                
            case 3:
                tampilkanKeranjang(keranjang);
                pause();
                break;
                
            case 4:
                if (keranjang == nullptr) {
                    cout << "Keranjang kosong!" << endl;
                } else {
                    int total = hitungTotalBayar(keranjang);
                    int bayar;
                    
                    cout << "\nTotal Belanja: Rp " << total << endl;
                    cout << "Jumlah Bayar: Rp ";
                    cin >> bayar;
                    
                    if (bayar >= total) {
                        cout << "Kembalian: Rp " << (bayar - total) << endl;
                        cetakStruk(keranjang, total);
                        
                        // Update stok ke database
                        Item* temp = keranjang;
                        while (temp != nullptr) {
                            Node* menu = cariMenu(root, temp->nama);
                            if (menu != nullptr) {
                                updateStokMenu(root, menu->data.nama, menu->data.stok);
                            }
                            temp = temp->next;
                        }
                        
                        bersihkanKeranjang(keranjang);
                    } else {
                        cout << "Uang tidak cukup!" << endl;
                    }
                }
                pause();
                break;
                
            case 5:
                if (keranjang != nullptr) {
                    // Kembalikan stok
                    Item* temp = keranjang;
                    while (temp != nullptr) {
                        Node* menu = cariMenu(root, temp->nama);
                        if (menu != nullptr) {
                            menu->data.stok += temp->jumlah;
                        }
                        temp = temp->next;
                    }
                    
                    bersihkanKeranjang(keranjang);
                }
                cout << "Transaksi dibatalkan!" << endl;
                pause();
                break;
                
            case 6:
                if (keranjang != nullptr) {
                    cout << "Keranjang masih ada isi. Batalkan transaksi dulu!" << endl;
                    pause();
                } else {
                    cout << "Kembali ke menu utama..." << endl;
                }
                break;
                
            default:
                cout << "Pilihan tidak valid!" << endl;
                pause();
        }
        
    } while (pilihan != 6 || keranjang != nullptr);
}

int main() {
    // Koneksi ke database PostgreSQL di Laragon
    // Sesuaikan dengan konfigurasi Laragon Anda
    if (!connectDatabase("localhost", "postgres", "password", "kasir_db", 5432)) {
        cout << "Gagal terhubung ke database. Program dihentikan." << endl;
        return 1;
    }
    
    // Login
    if (!login()) {
        disconnectDatabase();
        return 1;
    }
    
    // Load menu dari database
    Node* root = nullptr;
    vector<Menu> daftarMenu = ambilSemuaMenu();
    for (const auto& menu : daftarMenu) {
        root = tambahMenu(root, menu.nama, menu.kategori, menu.harga, menu.stok);
    }
    
    // Menu utama
    int pilihan;
    do {
        clearScreen();
        tampilkanMenuUtama();
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                menuKelolaMenu(root);
                break;
                
            case 2:
                menuTransaksi(root);
                break;
                
            case 3:
                cout << "\nTerima kasih telah menggunakan sistem kasir!" << endl;
                break;
                
            default:
                cout << "Pilihan tidak valid!" << endl;
                pause();
        }
        
    } while (pilihan != 3);
    
    // Cleanup
    hapusSemuaNode(root);
    disconnectDatabase();
    
    return 0;
}