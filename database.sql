CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    role TEXT DEFAULT 'kasir',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS menu (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nama TEXT NOT NULL UNIQUE,
    kategori TEXT NOT NULL,
    harga INTEGER NOT NULL,
    stok INTEGER NOT NULL DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

INSERT OR IGNORE INTO users (username, password, role) VALUES 
('kasir', 'kasir123', 'kasir');

INSERT OR IGNORE INTO menu (nama, kategori, harga, stok) VALUES
('Nasi Goreng', 'Makanan', 15000, 50),
('Mie Goreng', 'Makanan', 12000, 30),
('Ayam Bakar', 'Makanan', 25000, 20),
('Soto Ayam', 'Makanan', 18000, 25),
('Teh Manis', 'Minuman', 5000, 100),
('Jeruk Peras', 'Minuman', 8000, 50),
('Es Teh', 'Minuman', 5000, 100),
('Kopi Hitam', 'Minuman', 7000, 75);
