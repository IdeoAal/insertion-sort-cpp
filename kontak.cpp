#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <fstream>
#include <stdexcept>

using namespace std;

struct Kontak {
    string nama;
    string nomorTelepon;
};

string keHurufKecil(const string& teks) {
    string hasil = teks;
    for (size_t i = 0; i < hasil.size(); ++i) {
        hasil[i] = static_cast<char>(tolower(static_cast<unsigned char>(hasil[i])));
    }
    return hasil;
}

void insertionSortKontak(vector<Kontak>& daftar) {
    int n = daftar.size();
    for (int i = 1; i < n; ++i) {
        Kontak key = daftar[i];
        string keyKecil = keHurufKecil(key.nama);
        int j = i - 1;

        while (j >= 0 && keHurufKecil(daftar[j].nama) > keyKecil) {
            daftar[j + 1] = daftar[j];
            j = j - 1;
        }
        daftar[j + 1] = key;
    }
}

void simpanKontakKeFile(const vector<Kontak>& daftar, const string& namaFile = "kontak.txt") {
    ofstream file(namaFile);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk menyimpan kontak.\n";
        return;
    }

    for (size_t i = 0; i < daftar.size(); ++i) {
        file << daftar[i].nama << "|" << daftar[i].nomorTelepon << '\n';
    }
}

void muatKontakDariFile(vector<Kontak>& daftar, const string& namaFile = "kontak.txt") {
    ifstream file(namaFile);
    if (!file.is_open()) {
        return;
    }

    daftar.clear();
    Kontak k;
    while (getline(file, k.nama, '|') && getline(file, k.nomorTelepon)) {
        if (!k.nama.empty()) {
            daftar.push_back(k);
        }
    }

    insertionSortKontak(daftar);
}

void validasiKontak(const Kontak& k) {
    if (k.nama.empty() || k.nomorTelepon.empty()) {
        throw invalid_argument("Nama dan nomor telepon tidak boleh kosong.");
    }

    if (k.nama.find('|') != string::npos || k.nomorTelepon.find('|') != string::npos) {
        throw invalid_argument("Karakter '|' tidak diperbolehkan karena dipakai sebagai pemisah file.");
    }

    for (size_t i = 0; i < k.nomorTelepon.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(k.nomorTelepon[i]);
        if (isalpha(ch)) {
            throw invalid_argument("Nomor telepon tidak boleh mengandung huruf.");
        }
    }
}

void tambahKontak(vector<Kontak>& daftar) {
    Kontak kontakBaru;

    cout << "Masukkan Nama Kontak: ";
    getline(cin, kontakBaru.nama);

    cout << "Masukkan Nomor Telepon: ";
    getline(cin, kontakBaru.nomorTelepon);

    try {
        validasiKontak(kontakBaru);

        daftar.push_back(kontakBaru);
        insertionSortKontak(daftar);
        simpanKontakKeFile(daftar);

        cout << "Kontak tersimpan: " << kontakBaru.nama << " (" << kontakBaru.nomorTelepon << ")\n";
    } catch (const invalid_argument& e) {
        cout << "[Error Validasi] " << e.what() << " Kontak tidak disimpan.\n";
    }
}

void tampilkanKontak(const vector<Kontak>& daftar) {
    if (daftar.empty()) {
        cout << "Daftar kontak masih kosong.\n";
        return;
    }

    cout << "\n=== Daftar Kontak (A - Z) ===\n";
    for (size_t i = 0; i < daftar.size(); ++i) {
        cout << i + 1 << ". " << daftar[i].nama
             << " - " << daftar[i].nomorTelepon << '\n';
    }
}

int main() {
    vector<Kontak> daftarKontak;
    muatKontakDariFile(daftarKontak);

    int pilihan;
    do {
        cout << "\n=== Menu Kontak CLI ===\n";
        cout << "1. Tambah Kontak\n";
        cout << "2. Lihat Daftar Kontak\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input menu harus berupa angka.\n";
            pilihan = 1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (pilihan == 1) {
            tambahKontak(daftarKontak);
        } else if (pilihan == 2) {
            tampilkanKontak(daftarKontak);
        } else if (pilihan != 0) {
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    simpanKontakKeFile(daftarKontak);
    return 0;
}