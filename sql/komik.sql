-- Buat database
CREATE DATABASE IF NOT EXISTS komik_app;
USE komik_app;

-- Tabel users
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('admin', 'user') DEFAULT 'user',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel genres
CREATE TABLE IF NOT EXISTS genres (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel authors
CREATE TABLE IF NOT EXISTS authors (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel comics (data komik utama)
CREATE TABLE IF NOT EXISTS comics (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(100) NOT NULL,
    genre VARCHAR(100) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Tabel user_favorites (favorit user)
CREATE TABLE IF NOT EXISTS user_favorites (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    comic_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (comic_id) REFERENCES comics(id) ON DELETE CASCADE,
    UNIQUE KEY unique_favorite (user_id, comic_id)
);

-- Masukkan data dummy untuk testing
INSERT INTO users (username, password, role) VALUES 
('admin', 'admin123', 'admin'),
('user1', 'user123', 'user');

INSERT INTO genres (name) VALUES 
('Action'), ('Romance'), ('Comedy'), ('Horror'), ('Fantasy');

INSERT INTO authors (name) VALUES 
('Masashi Kishimoto'), ('Eiichiro Oda'), ('Akira Toriyama'), 
('Hajime Isayama'), ('Kentaro Miura');

INSERT INTO comics (title, author, genre) VALUES 
('Naruto', 'Masashi Kishimoto', 'Action'),
('One Piece', 'Eiichiro Oda', 'Action'),
('Dragon Ball', 'Akira Toriyama', 'Action'),
('Attack on Titan', 'Hajime Isayama', 'Action'),
('Berserk', 'Kentaro Miura', 'Fantasy');

-- Masukkan data favorite
INSERT INTO user_favorites (user_id, comic_id) VALUES 
(2, 1), (2, 2), (2, 3);