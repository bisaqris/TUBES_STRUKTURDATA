# Sistem Kasir Restoran

Aplikasi kasir restoran berbasis C++ dengan struktur data Binary Search Tree (BST) dan database SQLite untuk manajemen menu, transaksi, dan laporan penjualan, untuk Tugas Besar Mata Kuliah Struktur Data.

## Daftar Isi

- [Fitur](#fitur)
- [Struktur Data](#struktur-data)
- [Tech Stack](#tech-stack)
- [Instalasi](#instalasi)
- [Cara Penggunaan](#cara-penggunaan)
- [Struktur Tugas Besar](#struktur-tugas-besar)
- [Database Schema](#database-schema)
- [Output Overview](#output-overview)
- [Kontributor](#kontributor)

## Fitur

### 1. Sistem Login
- Autentikasi pengguna dengan username dan password
- Maksimal 3 kali percobaan login
- Password tersembunyi saat input (ditampilkan sebagai `*`)

### 2. Kelola Menu
- **Tambah Menu**: Menambahkan menu baru dengan nama, kategori, harga, dan stok
- **Tampilkan Menu**: Menampilkan daftar menu dengan traversal **In-Order** BST (terurut alfabetis)
- **Cari Menu**: Mencari menu berdasarkan nama menggunakan BST
- **Update Menu**: Mengubah kategori, harga, dan stok menu
- **Hapus Menu**: Menghapus menu dari sistem

### 3. Kelola Transaksi
- Menambahkan item ke keranjang belanja
- Melihat daftar belanja dalam keranjang
- Checkout dengan validasi pembayaran
- Perhitungan otomatis kembalian
- Cetak struk pembelian
- Pembatalan transaksi dengan pengembalian stok

### 4. Laporan Penjualan
- **Ringkasan Penjualan**: Total penjualan hari ini dan bulan ini
- **Riwayat Transaksi**: Daftar semua transaksi dengan traversal **Post-Order** BST
- **Detail Transaksi**: Melihat detail item dalam transaksi tertentu
- **Menu Terlaris**: Top 10 menu terlaris dengan traversal **Pre-Order** BST

## Struktur Data

### Binary Search Tree (BST)
Aplikasi menggunakan 3 jenis BST:

1. **BST Menu** (berdasarkan nama menu)
   - Traversal: In-Order untuk menampilkan menu terurut alfabetis
   
2. **BST Transaksi** (berdasarkan ID transaksi)
   - Traversal: Post-Order untuk menampilkan riwayat transaksi
   
3. **BST Menu Terlaris** (berdasarkan jumlah terjual)
   - Traversal: Pre-Order untuk menampilkan menu terlaris

### Linked List
- Digunakan untuk keranjang belanja (shopping cart)
- Setiap node menyimpan item, jumlah, harga satuan, dan total harga

## Tech Stack

- **Bahasa**: C++
- **Database**: SQLite3
- **Compiler**: G++ (MinGW/GCC)
- **Platform**: Windows

## Instalasi

### Prasyarat
- C++ Compiler (MinGW/GCC)
- SQLite3 Library

### Langkah Instalasi

1. **Clone repository**
```bash
git clone [https://github.com/bisaqris/TUBES_STRUKTURDATA.git](https://github.com/bisaqris/TUBES_STRUKTURDATA.git)
cd TUBES_STRUKTURDATA
```

2. **Install SQLite3** (jika belum terinstall)

**Windows (MinGW):**
```bash
# Download SQLite3 dari https://www.sqlite.org/download.html
# Extract dan letakkan di folder include dan lib compiler
```

**Linux:**
```bash
sudo apt-get install libsqlite3-dev
```

**MacOS:**
```bash
brew install sqlite3
```

3. **Compile program**

**Windows (MinGW):**
```bash
g++ -o kasir main.cpp database.cpp binarySearchTree.cpp auth.cpp viewKasir.cpp cart.cpp -lsqlite3
```

**Linux/MacOS:**
```bash
g++ -o kasir main.cpp auth.cpp binarySearchTree.cpp cart.cpp database.cpp viewKasir.cpp -lsqlite3
```

4. **Jalankan program**
```bash
./kasir        # Linux/MacOS
kasir.exe      # Windows
```

## Cara Penggunaan

### Login
- **Username**: `kasir`
- **Password**: `kasir123`

### Menu Utama
```
========================================
        SISTEM KASIR RESTORAN
========================================
1. Kelola Menu
2. Kelola Transaksi
3. Laporan Penjualan
0. Keluar
========================================
```

### Contoh Penggunaan

**1. Menambah Menu Baru**
```
Pilih menu: 1 (Kelola Menu)
Pilih menu: 1 (Tambah Menu)
Nama Menu: Bakso
Kategori: Makanan
Harga: 15000
Stok: 30
```

**2. Melakukan Transaksi**
```
Pilih menu: 2 (Kelola Transaksi)
Pilih menu: 2 (Tambah Item ke Keranjang)
Nama menu: Nasi Goreng
Jumlah: 2
```

**3. Checkout**
```
Pilih menu: 4 (Checkout)
Masukkan jumlah bayar: Rp 50000
Kembalian: Rp 20000
```

## Struktur Tugas Besar

```
TUBES_STRUKTURDATA/
├── main.cpp                 # Entry point program
├── kasir.h                  # Header file utama
├── auth.cpp                 # Implementasi login & autentikasi
├── binarySearchTree.cpp     # Implementasi BST untuk Menu, Transaksi, Menu Terlaris
├── cart.cpp                 # Implementasi keranjang belanja (Linked List)
├── database.cpp             # Implementasi database SQLite3
├── viewKasir.cpp            # Implementasi UI dan menu program
├── database.sql             # Schema database
├── kasir.db                 # File database SQLite (dibuat otomatis)
└── README.md                # Dokumentasi proyek
```

## Database Schema

### Tabel `users`
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    role TEXT DEFAULT 'kasir',
    created_at DATETIME DEFAULT (datetime('now', '+7 hours'))
);
```

### Tabel `menu`
```sql
CREATE TABLE menu (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nama TEXT NOT NULL UNIQUE,
    kategori TEXT NOT NULL,
    harga INTEGER NOT NULL,
    stok INTEGER NOT NULL DEFAULT 0,
    created_at DATETIME DEFAULT (datetime('now', '+7 hours')),
    updated_at DATETIME DEFAULT (datetime('now', '+7 hours'))
);
```

### Tabel `transaksi`
```sql
CREATE TABLE transaksi (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    tanggal DATETIME DEFAULT (datetime('now', '+7 hours')),
    total_bayar INTEGER NOT NULL,
    uang_diterima INTEGER NOT NULL,
    kembalian INTEGER NOT NULL
);
```

### Tabel `detail_transaksi`
```sql
CREATE TABLE detail_transaksi (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    transaksi_id INTEGER NOT NULL,
    nama_menu TEXT NOT NULL,
    jumlah INTEGER NOT NULL,
    harga_satuan INTEGER NOT NULL,
    subtotal INTEGER NOT NULL,
    FOREIGN KEY (transaksi_id) REFERENCES transaksi(id)
);
```

## Output Overview

### Login Screen
```
========================================
           SISTEM LOGIN KASIR
========================================

Username: kasir
Password: *********

Login berhasil! Selamat datang, kasir!
```

### Menu Utama
```
========================================
        SISTEM KASIR RESTORAN
========================================
1. Kelola Menu
2. Kelola Transaksi
3. Laporan Penjualan
0. Keluar
========================================
```

### Daftar Menu (In-Order Traversal)
```
========================================
         DAFTAR MENU (In-Order)
========================================
Nama                Kategori       Harga          Stok
----------------------------------------
Ayam Bakar          Makanan        Rp 25000       Stok: 20
Es Teh              Minuman        Rp 5000        Stok: 100
Jeruk Peras         Minuman        Rp 8000        Stok: 50
Kopi Hitam          Minuman        Rp 7000        Stok: 75
Mie Goreng          Makanan        Rp 12000       Stok: 30
Nasi Goreng         Makanan        Rp 15000       Stok: 50
Soto Ayam           Makanan        Rp 18000       Stok: 25
Teh Manis           Minuman        Rp 5000        Stok: 100
```

## Fitur Struktur Data

### Binary Search Tree Features
- **Insertion**: O(log n) average case
- **Search**: O(log n) average case
- **Deletion**: O(log n) average case
- **Traversal**: In-Order, Pre-Order, Post-Order

### Linked List Features
- **Dynamic Size**: Keranjang belanja dapat menampung unlimited items
- **Easy Insertion**: Menambah item baru di akhir list
- **Duplicate Handling**: Otomatis menggabungkan item yang sama

## Kontributor

- **Abu Abdirrahman Humaid Al-Atsary** - 103122400020
- **Najwa Areefa Ghaisani** - 103122400028
- **Muhammad Rasyid Ridho** - 103122400018
