# Final Project Struktur Data - Kelompok 1

## Nama Kelompok:

| NRP         | Nama                                 |
|-------------|--------------------------------------|
| 5027241056  | Theodorus Aaron Ugraha               |
| 5027241102  | Rayhan Agnan Kusuma                  |
| 5027241071  | Fika Arka Nuriyah                    |
| 5027241118  | Naufal Ardhana                       |
| 5027241105  | Naruna Vicranthyo Putra Gangga       |
| 5027241065  | Dina Rahmadani                       |

## Deskripsi Proyek
Implementasi sistem pencarian rute dengan mempertimbangkan preferensi pengguna menggunakan algoritma A* dan Dijkstra.

## Struktur Proyek
```
.
├── asset/              # Dokumentasi proyek
├── data/              # File data (CSV dan JSON)
│   ├── location.csv
│   ├── preference.json
│   └── route.csv
├── include/           # Header files (.hpp)
│   ├── algorithm/     # Implementasi algoritma
│   ├── graph/        # Struktur data graph
│   ├── io/           # File handlers
│   ├── preference/   # Pengolahan preferensi
│   └── utils/        # Utilitas
├── src/              # Source files (.cpp)
├── test/             # Unit tests
└── main.cpp          # Entry point
```

## Fitur
1. Pencarian Rute:
  - Menggunakan dua algoritma graf terkenal:
      - A* (A-Star)
      - Dijkstra
  - Tujuannya adalah untuk menemukan rute terbaik antara lokasi berdasarkan data graf.
2. Preferensi Pengguna:
  - Sistem mempertimbangkan preferensi pengguna dalam menentukan rute terbaik (misalnya: rute terpendek, tercepat, atau teraman).
  - Preferensi ini dibaca dari file JSON (preference.json).
3. Pembacaan File Eksternal:
  - Data lokasi dan rute dibaca dari file:
      - location.csv
      - route.csv
      - preference.json
4. Modularitas:
  - Struktur proyek terorganisir dalam beberapa direktori:
      - algorithm/ — untuk algoritma graf
      - graph/ — struktur data graf
      - io/ — pembacaan file
      - preference/ — pengolahan preferensi
      - utils/ — fungsi utilitas
      - test/ — unit test
5. Unit Testing:
  - Proyek menyediakan unit tests untuk memastikan akurasi dari algoritma pencarian rute.
6. Kompilasi & Eksekusi Sederhana:
  - Kompilasi: make
  - Menjalankan program: ./main
  - Menjalankan test: make test (kemungkinan dilanjutkan setelah bagian yang terpotong)

## Cara Kompilasi
Proyek ini menggunakan makefile, jadi bisa mengompilasinya dengan perintah berikut dari terminal:
```bash
make
```
> Pastikan kamu memiliki make dan compiler C++ seperti g++ terpasang.

## Cara Menjalankan
Setelah dikompilasi (jika tidak diubah makefile-nya), file output biasanya akan bernama seperti main atau nama lain yang ditentukan. Misalnya, jika output-nya adalah main, jalankan dengan:
```bash
./main
```
> File eksekusi bisa saja bernama berbeda; kita bisa cek isi makefile untuk kepastiannya jika kamu mau.

## Cara Menjalankan Tests
```bash
make test
```
