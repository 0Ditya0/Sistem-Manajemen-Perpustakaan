#include <iostream>
#include <string>
#include <ctime> // untuk waktu real-time
#include <cctype> // untuk fungsi isdigit
using namespace std;

struct Buku {
    string judul;
    string penulis;
    bool tersedia;
    string kategori;
    Buku* next;
};

struct Perpustakaan {
    Buku* head;
};

struct Transaksi {
    string namaAnggota;
    string nimAnggota;
    string judulBuku;
    time_t tanggalPinjam;
    time_t tanggalKembali;
    Transaksi* next;
};

struct LogTransaksi {
    Transaksi* head;
};

void inisialisasiPerpustakaan(Perpustakaan &perpus) {
    perpus.head = nullptr;
}

void tambahBuku(Perpustakaan &perpus, string judul, string penulis, string kategori) {
    Buku* bukuBaru = new Buku;
    bukuBaru->judul = judul;
    bukuBaru->penulis = penulis;
    bukuBaru->kategori = kategori;
    bukuBaru->tersedia = true;
    bukuBaru->next = perpus.head;
    perpus.head = bukuBaru;
    cout << "Buku berhasil ditambahkan." << endl;
    getchar();
    system("cls");
}

void tampilkanBuku(Perpustakaan perpus) {
    system("cls");
    cout << "Daftar Buku:" << endl;
    Buku* current = perpus.head;
    int index = 1;
    if (current == nullptr) {
        cout << "Tidak ada buku yang tersedia." << endl;
        cout << "--------------------" << endl;
        return;
    }

    while (current != nullptr) {
        cout << index << ". " << current->judul << " oleh " << current->penulis << " (" << current->kategori << ")";
        if (current->tersedia) {
            cout << " (Tersedia)";
        } else {
            cout << " (Tidak Tersedia)";
        }
        cout << endl;
        cout << "--------------------" << endl;
        current = current->next;
        index++;
    }

    cout << "\n\nKlik Enter Untuk Melanjutkan\n\n";
    cin.ignore();
    getchar();
    system("cls");
}

void tampilkanBukuBerdasarkanKategori(Perpustakaan perpus, string kategori) {
    system("cls");
    bool found = false;
    Buku* current = perpus.head;
    int index = 1;
    if (current == nullptr) {
        cout << "Tidak ada buku yang tersedia." << endl;
        cout << "--------------------" << endl;
        return;
    }
    
    while (current != nullptr) {
        if (current->kategori == kategori) {
            cout << index << ". " << current->judul << " oleh " << current->penulis;
            if (current->tersedia) {
                cout << " (Tersedia)";
            } else {
                cout << " (Tidak Tersedia)";
            }
            cout << endl;
            cout << "--------------------" << endl;
            found = true;
        }
        current = current->next;
        index++;
    }
    if (!found) {
        cout << "Tidak ada buku dalam kategori " << kategori << "." << endl;
    }

    cout << "\n\nKlik Enter Untuk Melanjutkan\n\n";
    cin.ignore();
    getchar();
    system("cls");
}

Buku* cariBuku(Perpustakaan perpus, string judul) {
    Buku* current = perpus.head;
    while (current != nullptr) {
        if (current->judul == judul) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // buku tidak ditemukan
}

void pinjamBuku(Perpustakaan &perpus, LogTransaksi &log, string namaAnggota, string nimAnggota, string judulBuku) {
    Buku* buku = cariBuku(perpus, judulBuku);
    if (buku != nullptr) {
        if (buku->tersedia) {
            Transaksi* transaksi = new Transaksi;
            transaksi->namaAnggota = namaAnggota;
            transaksi->nimAnggota = nimAnggota;
            transaksi->judulBuku = judulBuku;
            time(&transaksi->tanggalPinjam);
            transaksi->tanggalKembali = transaksi->tanggalPinjam + 7 * 24 * 60 * 60; // 1 minggu
            transaksi->next = log.head;
            log.head = transaksi;
            buku->tersedia = false;

            // Tampilkan tanggal pengembalian
            char buffer[80];
            struct tm * timeinfo;
            timeinfo = localtime(&transaksi->tanggalKembali);
            strftime(buffer, 80, "%d-%m-%Y", timeinfo);
            cout << "Buku berhasil dipinjam. Tanggal pengembalian: " << buffer << endl;
        } else {
            cout << "Buku tidak tersedia untuk dipinjam." << endl;
        }
    } else {
        cout << "Buku tidak ditemukan." << endl;
    }
    getchar();
    system("cls");
}

void kembalikanBuku(Perpustakaan &perpus, LogTransaksi &log, string judulBuku) {
    Buku* buku = cariBuku(perpus, judulBuku);
    if (buku != nullptr) {
        Transaksi* current = log.head;
        while (current != nullptr) {
            if (current->judulBuku == judulBuku && difftime(time(0), current->tanggalKembali) <= 0) {
                buku->tersedia = true;
                current->tanggalKembali = time(0); // Update return date to current time
                cout << "Buku berhasil dikembalikan." << endl;
                getchar();
                system("cls");
                return;
            }
            current = current->next;
        }
        cout << "Buku tidak dipinjam atau sudah dikembalikan." << endl;
    } else {
        cout << "Buku tidak ditemukan." << endl;
    }
    getchar();
    system("cls");
}

void hapusBuku(Perpustakaan &perpus, string judul) {
    Buku* current = perpus.head;
    Buku* previous = nullptr;

    while (current != nullptr && current->judul != judul) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Buku tidak ditemukan." << endl;
    } else {
        if (previous == nullptr) {
            // Buku yang dihapus adalah buku pertama
            perpus.head = current->next;
        } else {
            previous->next = current->next;
        }
        delete current;
        cout << "Buku berhasil dihapus." << endl;
    }
    getchar();
    system("cls");
}

void updateBuku(Perpustakaan &perpus, string judul) {
    Buku* buku = cariBuku(perpus, judul);
    if (buku == nullptr) {
        cout << "Buku tidak ditemukan." << endl;
        getchar();
        system("cls");
        return;
    }

    cout << "Mengupdate informasi untuk buku: " << buku->judul << endl;
    cout << "Judul baru (kosongkan jika tidak ingin mengubah): ";
    string judulBaru;
    getline(cin, judulBaru);
    if (!judulBaru.empty()) {
        buku->judul = judulBaru;
    }

    cout << "Penulis baru (kosongkan jika tidak ingin mengubah): ";
    string penulisBaru;
    getline(cin, penulisBaru);
    if (!penulisBaru.empty()) {
        buku->penulis = penulisBaru;
    }

    cout << "Kategori baru (kosongkan jika tidak ingin mengubah): ";
    string kategoriBaru;
    getline(cin, kategoriBaru);
    if (!kategoriBaru.empty()) {
        buku->kategori = kategoriBaru;
    }


    cout << "Buku berhasil diperbarui." << endl;
    getchar();
    getchar();
    system("cls");
}

bool isValidNIM(const string& nim) {
    for (char c : nim) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void bubbleSort(Perpustakaan &perpus) {
    if (perpus.head == nullptr) return;

    bool swapped;
    Buku* ptr1;
    Buku* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = perpus.head;

        while (ptr1->next != lptr) {
            if (ptr1->judul > ptr1->next->judul) {
                swap(ptr1->judul, ptr1->next->judul);
                swap(ptr1->penulis, ptr1->next->penulis);
                swap(ptr1->kategori, ptr1->next->kategori);
                swap(ptr1->tersedia, ptr1->next->tersedia);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void tampilkanBukuDipinjam(LogTransaksi log, string nim) {
    Transaksi* current = log.head;
    bool found = false;
    cout << "Daftar Buku yang Dipinjam oleh NIM " << nim << ":" << endl;
    while (current != nullptr) {
        if (current->nimAnggota == nim) {
            cout << current->judulBuku << " (Dipinjam pada: " << ctime(&current->tanggalPinjam) << ")" << endl;
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        cout << "Tidak ada buku yang sedang dipinjam oleh NIM ini." << endl;
    }
    getchar();
    system("cls");
}



string getKategori(int kategoriIndex) {
    switch (kategoriIndex) {
        case 1: return "Biologi";
        case 2: return "Ekonomi";
        case 3: return "Matematika";
        case 4: return "Pemrograman";
        case 5: return "Bahasa Inggris";
        case 6: return "Fisika";
        case 7: return "Hukum";
        default: return "Kategori Tidak Valid";
    }
}

int main() {
    Perpustakaan perpustakaan;
    inisialisasiPerpustakaan(perpustakaan);

    LogTransaksi logTransaksi;
    logTransaksi.head = nullptr;

    int loginChoice, pilihan, pilihanKategori;
    string namaAnggota, nimAnggota, judulBuku, penulis, kategori;
    string username, nim;

    do {
    		cout << endl;
    cout << "=========================================";
    cout << "\n.........................................\n";     
    cout << ".............SYSTEM..START...............";
    cout << "\n.........................................";
    cout << "\n=========================================\n"
         << endl;
    getchar();
    system("cls");
    
    
        cout << endl;
        cout << "=== Aplikasi Perpustakaan ===\n";
        cout << "======= Login Sebagai =======\n";
        cout << "\n";
		cout << "1. Karyawan\n";
        cout << "2. Mahasiswa\n";
        cout << "3. Redaksional\n";        
        cout << "0. Keluar\n";
        cout << "\n";
		cout << "Pilih: ";
        cin >> loginChoice;
        cin.ignore();
        system("cls");

        switch (loginChoice) {
            case 1:
                cout << "Masukkan nama karyawan: ";
                getline(cin, username);
                cout << "Masukkan ID(angka): ";
                getline(cin, nim);
                cin.ignore();
                system("cls");
                // Validasi NIM karyawan
                if (isValidNIM(nim)) {
                    do {
                        cout << "\nMenu Karyawan:\n";
                        cout << "1. Tambah Buku\n";
                        cout << "2. Daftar Buku\n";
                        cout << "3. Daftar Buku berdasarkan Kategori\n";
                        cout << "4. Urutkan Buku berdasarkan Judul\n";
                        cout << "5. Hapus Buku\n";
                        cout << "6. Update Buku\n";
                        cout << "0. Keluar dari Karyawan\n";
                        cout << "Pilih: ";
                        cin >> pilihan;
                        cin.ignore();
                        
                    

                        switch (pilihan) {
                            case 1:
                                cout << "Judul: ";
                                getline(cin, judulBuku);
                                cout << "Penulis: ";
                                getline(cin, penulis);
                                cout << "Kategori:\n";
                                cout << "1. Biologi\n";
                                cout << "2. Ekonomi\n";
                                cout << "3. Matematika\n";
                                cout << "4. Pemrograman\n";
                                cout << "5. Bahasa Inggris\n";
                                cout << "6. Fisika\n";
                                cout << "7. Hukum\n";
                                cout << "Pilih Kategori: ";
                                cin >> pilihanKategori;
                                cin.ignore();
                                kategori = getKategori(pilihanKategori);
                                if (kategori != "Kategori Tidak Valid") {
                                    tambahBuku(perpustakaan, judulBuku, penulis, kategori);
                                } else {
                                    cout << "Kategori tidak valid." << endl;
                                }
                                break;
                            case 2:
                                system("cls");
                                tampilkanBuku(perpustakaan);
                                break;
                            case 3:
                                cout << "Pilih kategori:\n";
                                cout << "1. Biologi\n";
                                cout << "2. Ekonomi\n";
                                cout << "3. Matematika\n";
                                cout << "4. Pemrograman\n";
                                cout << "5. Bahasa Inggris\n";
                                cout << "6. Fisika\n";
                                cout << "7. Hukum\n";
                                cout << "Pilih Kategori: ";
                                cin >> pilihanKategori;
                                cin.ignore();
                                kategori = getKategori(pilihanKategori);
                                if (kategori != "Kategori Tidak Valid") {
                                    system("cls");
                                    tampilkanBukuBerdasarkanKategori(perpustakaan, kategori);
                                } else {
                                    cout << "Kategori tidak valid." << endl;
                                }
                                break;
                            case 4:
                                bubbleSort(perpustakaan);
                                cout << "Buku berhasil diurutkan berdasarkan judul." << endl;
                                getchar();
                                system("cls");
                                break;
                            case 5:
                                cout << "Judul Buku yang akan dihapus: ";
                                getline(cin, judulBuku);
                                hapusBuku(perpustakaan, judulBuku);
                                break;
                            case 6:
                                cout << "Judul Buku yang akan diupdate: ";
                                getline(cin, judulBuku);
                                updateBuku(perpustakaan, judulBuku);
                                break;
                            case 0:
                                cout << "Keluar dari menu karyawan." << endl;
                                getchar();
                                system("cls");
                                break;
                            default:
                                cout << "Pilihan tidak valid." << endl; 
                        }
                    } while (pilihan != 0);
                } else {
                    cout << "NIM tidak valid. Harus berupa angka." << endl;
                    getchar();
                    system("cls");
                }
                break;

            case 2:
                cout << "Masukkan nama mahasiswa: ";
                getline(cin, namaAnggota);
                cout << "Masukkan NIM mahasiswa (angka): ";
                getline(cin, nimAnggota);
                getchar();
                system("cls");
                // Validasi NIM mahasiswa
                if (isValidNIM(nimAnggota)) {
                    do {
                        cout << "\nMenu Mahasiswa:\n";
                        cout << "1. Daftar Buku\n";
                        cout << "2. Pinjam Buku\n";
                        cout << "3. Kembalikan Buku\n";
                        cout << "4. Daftar Buku yang Sedang Dipinjam\n";
                        cout << "0. Keluar\n";
                        cout << "Pilih: ";
                        cin >> pilihan;
                        cin.ignore();

                        switch (pilihan) {
                            case 1:
                                system("cls");
                                tampilkanBuku(perpustakaan);
                                break;
                            case 2:
                                cout << "Judul Buku yang akan dipinjam: ";
                                getline(cin, judulBuku);
                                pinjamBuku(perpustakaan, logTransaksi, namaAnggota, nimAnggota, judulBuku);
                                break;
                            case 3:
                                cout << "Judul Buku yang akan dikembalikan: ";
                                getline(cin, judulBuku);
                                kembalikanBuku(perpustakaan, logTransaksi, judulBuku);
                                break;
                            case 4:
                                system("cls");
                                tampilkanBukuDipinjam(logTransaksi, nimAnggota);
                                break;
                            case 0:
                                cout << "Terimakasih telah mengunjungi perpustakaan kami." << endl;
                                getchar();
                                system("cls");
                                break;
                            default:
                                cout << "Pilihan tidak valid." << endl;
                        }
                    } while (pilihan != 0);
                } else {
                    cout << "NIM tidak valid. Harus berupa angka." << endl;
                    getchar();
                    system("cls");
                }
                break;
			
            case 3:
                	cout << endl;
    				cout << "\n========================================="
         				 << endl;
    				cout << "KELOMPOK 4 - MANAJEMEN PERPUSTAKAAN "
        				 << endl;
   				    cout << "=========================================\n\n"
        				 << endl;
   					cout << "23.11.5393 - FARIS ANDI MUHAMMAD" << endl;
    				cout << "23.11.5391 - RADITYA JULIAN PRIMASAKTI" << endl;
    				cout << "23.11.5427 - KRISNA KHARISMAWAN NUGRAHA" << endl;
    				cout << "23.11.5410 - ATTILA ARYA R PATADJENU" << endl;
    				cout << "23.11.5375 - ZAIDAN HABIBI" << endl;
    				cout << "\n\n=========================================\n\n"
         				 << endl;
    				cout << "\n\nKlik Enter Untuk Melanjutkan\n"
         				 << endl;
    				getchar();
    				system("cls");
            case 0:
                cout << "Keluar dari program." << endl;
                getchar();
                system("cls");
                break;
			
            default:
                cout << "Pilihan tidak valid." << endl;
                getchar();
                system("cls");
        }
    } while (loginChoice != 0);

    return 0;
}
