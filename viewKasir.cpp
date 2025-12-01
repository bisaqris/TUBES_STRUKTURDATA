#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

void menuUtama()
{
    Node *root = nullptr;
    vector<Menu> daftarMenu = ambilSemuaMenu();
    for (const auto &menu : daftarMenu)
    {
        root = tambahMenu(root, menu.nama, menu.kategori, menu.harga, menu.stok);
    }
    int pilihan;
    do
    {
        clearScreen();
        cout << "\n========================================" << endl;
        cout << "        SISTEM KASIR RESTORAN" << endl;
        cout << "========================================" << endl;
        cout << "1. Kelola Menu" << endl;
        cout << "2. Kelola Transaksi" << endl;
        cout << "0. Keluar" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            menuAdmin(root);
            break;
        case 2:
            menuKasir(root);
            break;
        case 0:
            cout << "Terima kasih telah menggunakan sistem kasir!" << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
            pause();
        }
    } while (pilihan != 0);

    hapusSemuaNode(root);
}

void menuKasir(Node *root)
{
    Item *keranjang = nullptr;
    int pilihan;

    do
    {
        clearScreen();
        cout << "\n========================================" << endl;
        cout << "            MENU KASIR" << endl;
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

        switch (pilihan)
        {
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
            pause();
            break;
        case 2:
        {
            string nama;
            int jumlah;

            cout << "\nNama menu: ";
            getline(cin, nama);

            Node *menu = cariMenu(root, nama);
            if (menu != nullptr)
            {
                if (menu->data.stok > 0)
                {
                    cout << "Jumlah: ";
                    cin >> jumlah;

                    if (jumlah <= menu->data.stok)
                    {
                        tambahKeranjang(keranjang, nama, jumlah, menu->data.harga);
                        menu->data.stok -= jumlah;
                        cout << "Item berhasil ditambahkan ke keranjang!" << endl;
                    }
                    else
                    {
                        cout << "Stok tidak mencukupi! Stok tersedia: "
                             << menu->data.stok << endl;
                    }
                }
                else
                {
                    cout << "Maaf, stok habis!" << endl;
                }
            }
            else
            {
                cout << "Menu tidak ditemukan!" << endl;
            }
            pause();
            break;
        }
        case 3:
            tampilkanKeranjang(keranjang);
            cout << "\nTotal: Rp " << hitungTotalBayar(keranjang) << endl;
            pause();
            break;
        case 4:
        {
            if (keranjang == nullptr)
            {
                cout << "Keranjang kosong!" << endl;
            }
            else
            {
                int total = hitungTotalBayar(keranjang);
                int bayar;

                cetakStruk(keranjang, total);
                cout << "Masukkan jumlah bayar: Rp ";
                cin >> bayar;

                if (bayar >= total)
                {
                    cout << "Kembalian: Rp " << (bayar - total) << endl;
                    cout << "\nTransaksi berhasil!" << endl;

                    Item *temp = keranjang;
                    while (temp != nullptr)
                    {
                        Node *menu = cariMenu(root, temp->nama);
                        if (menu != nullptr)
                        {
                            updateMenuDatabase(menu->data);
                        }
                        temp = temp->next;
                    }

                    bersihkanKeranjang(keranjang);
                }
                else
                {
                    cout << "Uang tidak cukup!" << endl;
                }
            }
            pause();
            break;
        }
        case 5:
            Item *temp = keranjang;
            while (temp != nullptr)
            {
                Node *menu = cariMenu(root, temp->nama);
                if (menu != nullptr)
                {
                    menu->data.stok += temp->jumlah;
                }
                temp = temp->next;
            }
            bersihkanKeranjang(keranjang);
            cout << "Transaksi dibatalkan!" << endl;
            pause();
            break;
        }
    } while (pilihan != 0);

    bersihkanKeranjang(keranjang);
}