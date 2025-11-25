-- Buat database baru
CREATE DATABASE kasir_db;

-- Gunakan database
\c kasir_db;

-- Tabel untuk menu
CREATE TABLE menu (
    id SERIAL PRIMARY KEY,
    nama VARCHAR(100) UNIQUE NOT NULL,
    kategori VARCHAR(50) NOT NULL,
    harga INTEGER NOT NULL,
    stok INTEGER NOT NULL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel untuk users (kasir)
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    nama_lengkap VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel untuk transaksi
CREATE TABLE transaksi (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id),
    total_bayar INTEGER NOT NULL,
    uang_bayar INTEGER NOT NULL,
    kembalian INTEGER NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel untuk detail transaksi
CREATE TABLE detail_transaksi (
    id SERIAL PRIMARY KEY,
    transaksi_id INTEGER REFERENCES transaksi(id) ON DELETE CASCADE,
    menu_nama VARCHAR(100),
    jumlah INTEGER NOT NULL,
    harga_satuan INTEGER NOT NULL,
    total_harga INTEGER NOT NULL
);

-- Insert user default (admin/admin)
INSERT INTO users (username, password, nama_lengkap) 
VALUES ('admin', 'admin', 'Administrator');

-- Insert beberapa menu contoh
INSERT INTO menu (nama, kategori, harga, stok) VALUES
('Nasi Goreng', 'Makanan', 15000, 50),
('Mie Goreng', 'Makanan', 12000, 40),
('Ayam Bakar', 'Makanan', 25000, 30),
('Es Teh', 'Minuman', 5000, 100),
('Jus Jeruk', 'Minuman', 10000, 50),
('Kopi', 'Minuman', 8000, 80);

-- Index untuk performa
CREATE INDEX idx_menu_nama ON menu(nama);
CREATE INDEX idx_users_username ON users(username);
CREATE INDEX idx_transaksi_created ON transaksi(created_at);

-- View untuk laporan
CREATE VIEW view_menu_stok_rendah AS
SELECT nama, kategori, harga, stok
FROM menu
WHERE stok < 10
ORDER BY stok ASC;