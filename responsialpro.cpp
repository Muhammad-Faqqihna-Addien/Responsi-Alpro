#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Penyewa {
public:
    string id;
    string nama;
    string jenis;   
    int lama;       
    int total;      
    int diskon;     
    int bayar;      
    string bonus;

    Penyewa() {
        id = ""; nama = ""; jenis = "";
        lama = 0; total = 0; diskon = 0; bayar = 0; bonus = "";
    }

    void tampilkan() {
        cout << "ID Penyewa   : " << id << "\n";
        cout << "Nama         : " << nama << "\n";
        cout << "Jenis PS     : " << jenis << "\n";
        cout << "Lama Sewa    : " << lama << " jam\n";
        cout << "Total Awal   : Rp" << total << "\n";
        cout << "Potongan Disc: Rp" << diskon << "\n";
        cout << "Total Bayar  : Rp" << bayar << "\n";
        cout << "Bonus        : " << bonus << "\n";
        cout << "----------------------------------------\n";
    }
};

class SistemPenyewaan {
private:
    static const int MAX = 100;
    Penyewa data[MAX];
    int n;

    int hitungHargaPerJam(string jenis) {
        if (jenis == "PS3") return 10000;
        if (jenis == "PS4") return 15000;
        if (jenis == "PS5") return 20000;
        return 0;
    }

    int hitungDiskon(int totalAwal) {
        if (totalAwal >= 200000) return totalAwal * 0.20; 
        if (totalAwal >= 100000) return totalAwal * 0.10;
        return 0;
    }

    string hitungBonus(int lamaSewa) {
        if (lamaSewa >= 8) return "Gratis Mie + Minuman";
        if (lamaSewa >= 5) return "Gratis Minuman";
        return "Tidak Mendapat Bonus";
    }


    int rekursifPendapatan(int index) {
        if (index < 0) return 0;
        return data[index].bayar + rekursifPendapatan(index - 1);
    }

    int rekursifJam(int index) {
        if (index < 0) return 0;
        return data[index].lama + rekursifJam(index - 1);
    }


    void simpanKeFile() {
        ofstream file("data_rental.txt");
        if (!file) {
            cout << "[!] Gagal membuka file untuk menyimpan data.\n";
            return;
        }
        

        file << n << "\n"; 
        
        for (int i = 0; i < n; i++) {
            file << data[i].id << "\n";
            file << data[i].nama << "\n";
            file << data[i].jenis << "\n";
            file << data[i].lama << "\n";
            file << data[i].total << "\n";
            file << data[i].diskon << "\n";
            file << data[i].bayar << "\n";
            file << data[i].bonus << "\n";
        }
        file.close();
    }

    void muatDariFile() {
        ifstream file("data_rental.txt");
        if (!file) {

            n = 0;
            return;
        }

        if (!(file >> n)) {
            n = 0;
            return;
        }
        file.ignore(); 

        for (int i = 0; i < n; i++) {
            getline(file, data[i].id);
            getline(file, data[i].nama);
            file >> data[i].jenis;
            file >> data[i].lama;
            file >> data[i].total;
            file >> data[i].diskon;
            file >> data[i].bayar;
            file.ignore(); 
            getline(file, data[i].bonus);
        }
        file.close();
    }

public:
    SistemPenyewaan() { 
        muatDariFile(); 
    }

    void tambahPenyewa() {
        if (n >= MAX) {
            cout << "[!] Data rental penuh!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "          INPUT DATA PENYEWA\n";
        cout << "========================================\n";

        Penyewa p;
        cout << "ID Penyewa             : "; cin >> p.id;
        cin.ignore();

        cout << "Nama Penyewa           : "; getline(cin, p.nama);

        cout << "Jenis PS (PS3/PS4/PS5) : "; cin >> p.jenis;
        while (p.jenis != "PS3" && p.jenis != "PS4" && p.jenis != "PS5") {
            cout << "[!] Input salah. Masukkan PS3, PS4, atau PS5: ";
            cin >> p.jenis;
        }

        cout << "Lama Sewa (jam)        : "; cin >> p.lama;
        while (p.lama <= 0) {
            cout << "[!] Lama sewa tidak valid! Masukkan > 0 jam: ";
            cin >> p.lama;
        }

        p.total  = hitungHargaPerJam(p.jenis) * p.lama;
        p.diskon = hitungDiskon(p.total);
        p.bayar  = p.total - p.diskon;
        p.bonus  = hitungBonus(p.lama);

        data[n] = p;
        n++;

        
        simpanKeFile(); 

        cout << "\n[*] Data Berhasil Disimpan ke Database File!\n--- Ringkasan Transaksi ---\n";
        p.tampilkan();
    }

    void tampilkanSemua() {
        cout << "\n========================================\n";
        cout << "          DAFTAR SELURUH PENYEWA\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "[!] Belum ada transaksi sewa di database.\n";
            return;
        }

        for (int i = 0; i < n; i++) {
            cout << "------ Transaksi ke-" << (i + 1) << " ------\n";
            data[i].tampilkan();
        }
        cout << "Total data termuat: " << n << " penyewa\n";
    }

    void cariPenyewa() {
        cout << "\n========================================\n";
        cout << "           CARI DATA PENYEWA\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "[!] Belum ada data penyewa.\n";
            return;
        }

        string idCari;
        cout << "Masukkan ID Penyewa: "; cin >> idCari;

        bool ditemukan = false;
        for (int i = 0; i < n; i++) {
            if (data[i].id == idCari) {
                ditemukan = true;
                cout << "\n[+] Data Ditemukan:\n";
                data[i].tampilkan();
                break;
            }
        }

        if (!ditemukan)
            cout << "[!] ID '" << idCari << "' tidak ditemukan dalam sistem.\n";
    }

    void biayaTertinggi() {
        cout << "\n========================================\n";
        cout << "        PELANGGAN BIAYA TERTINGGI\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "[!] Belum ada data penyewa.\n";
            return;
        }

        int idxMax = 0;
        for (int i = 1; i < n; i++) {
            if (data[i].bayar > data[idxMax].bayar)
                idxMax = i;
        }
        data[idxMax].tampilkan();
    }

    void biayaTerendah() {
        cout << "\n========================================\n";
        cout << "         PELANGGAN BIAYA TERENDAH\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "[!] Belum ada data penyewa.\n";
            return;
        }

        int idxMin = 0;
        for (int i = 1; i < n; i++) {
            if (data[i].bayar < data[idxMin].bayar)
                idxMin = i;
        }
        data[idxMin].tampilkan();
    }

    void totalPendapatan() {
        cout << "\n========================================\n";
        cout << "         TOTAL PENDAPATAN RENTAL\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "Total Pendapatan : Rp0\n";
            return;
        }

        int total = rekursifPendapatan(n - 1);
        cout << "Jumlah Transaksi : " << n << " penyewa\n";
        cout << "Total Pendapatan : Rp" << total << "\n";
    }

    void totalJam() {
        cout << "\n========================================\n";
        cout << "           TOTAL JAM PENYEWAAN\n";
        cout << "========================================\n";

        if (n == 0) {
            cout << "Total Durasi Sewa: 0 jam\n";
            return;
        }

        int jam = rekursifJam(n - 1);
        cout << "Jumlah Transaksi : " << n << " penyewa\n";
        cout << "Total Waktu Sewa : " << jam << " jam\n";
    }

    void jalankan() {
        int pilihan;
        do {
            cout << "\n==========================================\n";
            cout << "         PENYEWAAN PLAYSTATION \n";
            cout << "==========================================\n";
            cout << "  1. Tambah Data Penyewa\n";
            cout << "  2. Tampilkan Semua Data\n";
            cout << "  3. Cari Penyewa (Berdasarkan ID)\n";
            cout << "  4. Tampilkan Pelanggan Biaya Tertinggi\n";
            cout << "  5. Tampilkan Pelanggan Biaya Terendah\n";
            cout << "  6. Total Pendapatan Rental\n";
            cout << "  7. Total Jam Penyewaan\n";
            cout << "  0. Keluar\n";
            cout << "------------------------------------------\n";
            cout << "  Pilih menu [0-7]: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1: tambahPenyewa();    break;
                case 2: tampilkanSemua();   break;
                case 3: cariPenyewa();      break;
                case 4: biayaTertinggi();   break;
                case 5: biayaTerendah();    break;
                case 6: totalPendapatan();  break;
                case 7: totalJam();         break;
                case 0: cout << "\n[*] Keluar dari program. Data aman tersimpan!\n"; break;
                default: cout << "[!] Pilihan menu tidak valid.\n";
            }
        } while (pilihan != 0);
    }
};

int main() {
    SistemPenyewaan sistem;
    sistem.jalankan();
    return 0;
}
