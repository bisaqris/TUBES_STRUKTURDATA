#include "kasir.h"

int main() {
    kasir* daftarMenu = nullptr;
    int totalPenjualan = 0;

    if (!login()) {
        cout << "Login gagal!\n";
        return 0;
    }

    int pilihan;
    do {
        cout << "\n=== MENU UTAMA KASIR ===\n";
        cout << "1. List Menu\n";
        cout << "2. Tambah Menu\n";
        cout << "3. Hapus Menu\n";
        cout << "4. Edit Menu / Update Stok\n";
        cout << "5. Pesanan Baru\n";
        cout << "6. Cetak Total Penjualan\n";
        cout << "7. Log Out\n";
        cout << "Pilih: "; cin >> pilihan;

        if (pilihan == 1) {
            tampilkanMenu(daftarMenu);
        }
        else if (pilihan == 2) {
            string nama, kategori;
            int harga, stok;
            cout << "Nama Menu : "; cin >> nama;
            cout << "Kategori (makanan/snack/minuman): "; cin >> kategori;
            cout << "Harga : "; cin >> harga;
            cout << "Stok : "; cin >> stok;
            daftarMenu = tambahMenu(daftarMenu, nama, kategori, harga, stok);
        }
        else if (pilihan == 3) {
            string nama;
            cout << "Nama menu yang akan dihapus: ";
            cin >> nama;
            daftarMenu = hapusMenu(daftarMenu, nama);
        }
        else if (pilihan == 4) {
            string nama; int stok;
            cout << "Nama menu: "; cin >> nama;
            cout << "Update stok menjadi: "; cin >> stok;
            updateStokMenu(daftarMenu, nama, stok);
        }
        else if (pilihan == 5) {
            item* keranjang = nullptr;
            string nama; int jumlah;
            char lanjut;

            do {
                cout << "Nama menu dipesan: ";
                cin >> nama;

                kasir* menuFound = cariMenu(daftarMenu, nama);
                if (!menuFound || menuFound->dataMenu.stok <= 0) {
                    cout << "Menu tidak tersedia!\n";
                } else {
                    cout << "Jumlah: ";
                    cin >> jumlah;

                    if (jumlah > menuFound->dataMenu.stok) {
                        cout << "Stok kurang!\n";
                    } else {
                        item* baru = new item;
                        baru->namaMenu = nama;
                        baru->jumlah = jumlah;
                        baru->hargaSatuan = menuFound->dataMenu.harga;
                        baru->totalHarga = jumlah * menuFound->dataMenu.harga;
                        baru->next = keranjang;
                        keranjang = baru;

                        menuFound->dataMenu.stok -= jumlah;
                    }
                }

                cout << "Pesan lagi? (y/n): ";
                cin >> lanjut;
            } while (lanjut == 'y');

            int totalBayar = hitungTotalBayar(keranjang);
            printStruk(keranjang, totalBayar);
            totalPenjualan += totalBayar;
        }
        else if (pilihan == 6) {
            cout << "Total penjualan hari ini: Rp " << totalPenjualan << endl;
        }

    } while (pilihan != 7);

    cout << "Berhasil Log Out!\n";
    return 0;
}
