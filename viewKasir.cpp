#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

void menuUtama() {
    Node* root = nullptr;
    vector<Menu> daftarMenu = ambilSemuaMenu();
    
    for (const auto& menu : daftarMenu) {
        root = tambahMenu(root, menu.nama, menu.kategori, menu.harga, menu.stok);
    }
    
    int pilihan;
    
    do {
        clearScreen();
        cout << "\n========================================" << endl;
        cout << "        SISTEM KASIR RESTORAN" << endl;
        cout << "========================================" << endl;
        cout << "1. Kelola Menu" << endl;
        cout << "2. Kelola Transaksi" << endl;
        cout << "3. Laporan Penjualan" << endl;
        cout << "0. Keluar" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                menuAdmin(root);
                break;
            case 2:
                menuKasir(root);
                break;
            case 3:
                menuLaporan();
                break;
            case 0:
                cout << "Terima kasih telah menggunakan sistem kasir!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
    
    hapusSemuaNode(root);
}

void menuAdmin(Node*& root) {
    int pilihan;
    
    do {
        cout << "\n========================================" << endl;
        cout << "           KELOLA MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. Tambah Menu" << endl;
        cout << "2. Tampilkan Menu (In-Order)" << endl;
        cout << "3. Cari Menu" << endl;
        cout << "4. Update Menu" << endl;
        cout << "5. Hapus Menu" << endl;
        cout << "0. Kembali" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                string nama, kategori;
                int harga, stok;
                
                cout << "\nNama Menu: ";
                getline(cin, nama);
                cout << "Kategori: ";
                getline(cin, kategori);
                cout << "Harga: ";
                cin >> harga;
                cout << "Stok: ";
                cin >> stok;
                
                Menu m = {nama, kategori, harga, stok};
                if (simpanMenuDatabase(m)) {
                    root = tambahMenu(root, nama, kategori, harga, stok);
                    cout << "Menu berhasil ditambahkan!" << endl;
                } else {
                    cout << "Gagal menambahkan menu!" << endl;
                }
                break;
            }
            case 2:
                cout << "\n========================================" << endl;
                cout << "         DAFTAR MENU (In-Order)" << endl;
                cout << "========================================" << endl;
                cout << left << setw(20) << "Nama" 
                     << setw(15) << "Kategori"
                     << setw(15) << "Harga"
                     << "Stok" << endl;
                cout << "----------------------------------------" << endl;
                tampilkanInOrder(root);
                break;
            case 3: {
                string nama;
                cout << "\nMasukkan nama menu yang dicari: ";
                getline(cin, nama);
                
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
                break;
            }
            case 4: {
                string nama, kategori;
                int harga, stok;
                
                cout << "\nMasukkan nama menu yang akan diupdate: ";
                getline(cin, nama);
                
                Node* node = cariMenu(root, nama);
                if (node != nullptr) {
                    cout << "Kategori baru: ";
                    getline(cin, kategori);
                    cout << "Harga baru: ";
                    cin >> harga;
                    cout << "Stok baru: ";
                    cin >> stok;
                    
                    node->data.kategori = kategori;
                    node->data.harga = harga;
                    node->data.stok = stok;
                    
                    if (updateMenuDatabase(node->data)) {
                        cout << "Menu berhasil diupdate!" << endl;
                    } else {
                        cout << "Gagal mengupdate menu di database!" << endl;
                    }
                } else {
                    cout << "Menu tidak ditemukan!" << endl;
                }
                break;
            }
            case 5: {
                string nama;
                cout << "\nMasukkan nama menu yang akan dihapus: ";
                getline(cin, nama);
                
                if (hapusMenuDatabase(nama)) {
                    root = hapusMenu(root, nama);
                    cout << "Menu berhasil dihapus!" << endl;
                } else {
                    cout << "Gagal menghapus menu!" << endl;
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
}

void menuKasir(Node* root) {
    Item* keranjang = nullptr;
    int pilihan;
    
    do {
        cout << "\n========================================" << endl;
        cout << "           KELOLA TRANSAKSI" << endl;
        cout << "========================================" << endl;
        cout << "1. Lihat Menu" << endl;
        cout << "2. Tambah Item ke Keranjang" << endl;
        cout << "3. Lihat Keranjang" << endl;
        cout << "4. Checkout" << endl;
        cout << "5. Batalkan Transaksi" << endl;
        cout << "0. Kembali" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1:
                cout << "\n========================================" << endl;
                cout << "            DAFTAR MENU" << endl;
                cout << "========================================" << endl;
                cout << left << setw(20) << "Nama" 
                     << setw(15) << "Kategori"
                     << setw(15) << "Harga"
                     << "Stok" << endl;
                cout << "----------------------------------------" << endl;
                tampilkanInOrder(root);                
                break;
            case 2: {
                string nama;
                int jumlah;
                
                cout << "\nNama menu: ";
                getline(cin, nama);
                
                Node* menu = cariMenu(root, nama);
                if (menu != nullptr) {
                    if (menu->data.stok > 0) {
                        cout << "Jumlah: ";
                        cin >> jumlah;
                        
                        if (jumlah <= menu->data.stok) {
                            tambahKeKeranjang(keranjang, nama, jumlah, menu->data.harga);
                            menu->data.stok -= jumlah;
                            cout << "Item berhasil ditambahkan ke keranjang!" << endl;
                        } else {
                            cout << "Stok tidak mencukupi! Stok tersedia: " 
                                 << menu->data.stok << endl;
                        }
                    } else {
                        cout << "Maaf, stok habis!" << endl;
                    }
                } else {
                    cout << "Menu tidak ditemukan!" << endl;
                }
                break;
            }
            case 3:
                tampilkanKeranjang(keranjang);
                cout << "\nTotal: Rp " << hitungTotalBayar(keranjang) << endl;
                break;
            case 4: {
                if (keranjang == nullptr) {
                    cout << "Keranjang kosong!" << endl;
                } else {
                    int total = hitungTotalBayar(keranjang);
                    int bayar;
                    
                    cetakStruk(keranjang, total);
                    cout << "Masukkan jumlah bayar: Rp ";
                    cin >> bayar;
                    
                    if (bayar >= total) {
                        int kembalian = bayar - total;
                        cout << "Kembalian: Rp " << kembalian << endl;
                        
                        // Simpan transaksi ke database
                        if (simpanTransaksi(keranjang, total, bayar, kembalian)) {
                            cout << "\nTransaksi berhasil disimpan!" << endl;
                            
                            // Update stok di database
                            Item* temp = keranjang;
                            while (temp != nullptr) {
                                Node* menu = cariMenu(root, temp->nama);
                                if (menu != nullptr) {
                                    updateMenuDatabase(menu->data);
                                }
                                temp = temp->next;
                            }
                            
                            bersihkanKeranjang(keranjang);
                        } else {
                            cout << "Gagal menyimpan transaksi!" << endl;
                        }
                    } else {
                        cout << "Uang tidak cukup!" << endl;
                    }
                }
                break;
            }
            case 5:
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
                cout << "Transaksi dibatalkan!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;    
        }
    } while (pilihan != 0);
    
    bersihkanKeranjang(keranjang);
}

void menuLaporan() {
    int pilihan;
    
    do {
        cout << "\n========================================" << endl;
        cout << "          LAPORAN PENJUALAN" << endl;
        cout << "========================================" << endl;
        cout << "1. Ringkasan Penjualan" << endl;
        cout << "2. Riwayat Transaksi" << endl;
        cout << "3. Detail Transaksi" << endl;
        cout << "4. Menu Terlaris" << endl;
        cout << "0. Kembali" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                int totalHariIni = hitungTotalPenjualanHariIni();
                int totalBulanIni = hitungTotalPenjualanBulanIni();
                
                cout << "\n========================================" << endl;
                cout << "       RINGKASAN PENJUALAN" << endl;
                cout << "========================================" << endl;
                cout << "Total Penjualan Hari Ini : Rp " << totalHariIni << endl;
                cout << "Total Penjualan Bulan Ini: Rp " << totalBulanIni << endl;
                cout << "========================================" << endl;
                break;
            }
            case 2: {
                vector<Transaksi> daftarTransaksi = ambilSemuaTransaksi();
                
                if (daftarTransaksi.empty()) {
                    cout << "\nBelum ada transaksi!" << endl;
                } else {
                    // Masukkan data ke BST
                    NodeTransaksi* rootTransaksi = nullptr;
                    for (const auto& t : daftarTransaksi) {
                        rootTransaksi = tambahTransaksi(rootTransaksi, t);
                    }
                    
                    cout << "\n========================================" << endl;
                    cout << "   RIWAYAT TRANSAKSI (Post-Order BST)" << endl;
                    cout << "========================================" << endl;
                    cout << left << setw(5) << "ID" 
                         << setw(22) << "Tanggal"
                         << setw(15) << "Total"
                         << setw(15) << "Bayar"
                         << "Kembali" << endl;
                    cout << "----------------------------------------" << endl;

                    tampilkanTransaksiPostOrder(rootTransaksi);
                    
                    cout << "========================================" << endl;
                    
                    hapusSemuaNodeTransaksi(rootTransaksi);
                }
                break;
            }
            case 3: {
                int idTransaksi;
                cout << "\nMasukkan ID Transaksi: ";
                cin >> idTransaksi;
                
                vector<DetailTransaksi> detail = ambilDetailTransaksi(idTransaksi);
                
                if (detail.empty()) {
                    cout << "Transaksi tidak ditemukan!" << endl;
                } else {
                    cout << "\n========================================" << endl;
                    cout << "    DETAIL TRANSAKSI #" << idTransaksi << endl;
                    cout << "========================================" << endl;
                    cout << left << setw(20) << "Menu" 
                         << setw(10) << "Qty"
                         << setw(15) << "Harga"
                         << "Subtotal" << endl;
                    cout << "----------------------------------------" << endl;
                    
                    int grandTotal = 0;
                    for (const auto& d : detail) {
                        cout << left << setw(20) << d.namaMenu
                             << setw(10) << d.jumlah
                             << "Rp " << setw(12) << d.hargaSatuan
                             << "Rp " << d.subtotal << endl;
                        grandTotal += d.subtotal;
                    }
                    
                    cout << "========================================" << endl;
                    cout << "TOTAL: Rp " << grandTotal << endl;
                    cout << "========================================" << endl;
                }
                break;
            }
            case 4: {
                vector<pair<string, int>> menuTerlaris = getMenuTerlaris(10);
                
                if (menuTerlaris.empty()) {
                    cout << "\nBelum ada data penjualan!" << endl;
                } else {

                    NodeMenuTerlaris* rootMenuTerlaris = nullptr;
                    for (const auto& menu : menuTerlaris) {
                        rootMenuTerlaris = tambahMenuTerlaris(rootMenuTerlaris, menu.first, menu.second);
                    }
                    
                    cout << "\n========================================" << endl;
                    cout << "     MENU TERLARIS (Pre-Order BST)" << endl;
                    cout << "========================================" << endl;
                    cout << left << setw(5) << "No" 
                         << setw(25) << "Nama Menu"
                         << "Terjual" << endl;
                    cout << "----------------------------------------" << endl;
                    
                    int nomor = 1;
                    tampilkanMenuTerlarisPreOrder(rootMenuTerlaris, nomor);
                    
                    cout << "========================================" << endl;

                    hapusSemuaNodeMenuTerlaris(rootMenuTerlaris);
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
        
    } while (pilihan != 0);
}