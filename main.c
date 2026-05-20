#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktur linked list untuk pasien
typedef struct Pasien {
    char nama[50];
    char gejala[100];
    int triage; // 1 = RED, 2 = YELLOW, 3 = GREEN
    struct Pasien* next;
} Pasien;

// pointer global untuk awal antrian
Pasien* head = NULL;

// fungsi utilitas untuk membersihkan newline dari fgets
void hapusNewline(char* string) {
    string[strcspn(string, "\n")] = 0;
}

// Fungsi utilitas untuk menyisipkan node ke dalam Priority Queue
void masukkanKeAntrian(Pasien* nodeBaru) {
    // jika antrian kosong, atau pasien baru punya prioritas lebih tinggi dari head
    if (head == NULL || head->triage > nodeBaru->triage) {
        nodeBaru->next = head;
        head = nodeBaru;
        return;
    }

    // traversal untuk mencari posisi yang tepat
    // tie-breaker: jika prioritas sama (<=), maka traversal lanjut agar pasien 
    // baru ditaruh di belakang pasien dengan prioritas yang sama (FIFO)
    Pasien* current = head;
    while (current->next != NULL && current->next->triage <= nodeBaru->triage) {
        current = current->next;
    }

    // sisipkan node di tengah/akhir
    nodeBaru->next = current->next;
    current->next = nodeBaru;
}

// Fitur 1: Registrasi Pasien Baru
void RegistrasiPasien() {
    Pasien* nodeBaru = (Pasien*)malloc(sizeof(Pasien));
    if (nodeBaru == NULL) {
        printf("Gagal mengalokasikan memori!\n");
        return;
    }

    printf("\n--- REGISTRASI PASIEN ---\n");
    printf("Nama Pasien    : ");
    fgets(nodeBaru->nama, 50, stdin);
    hapusNewline(nodeBaru->nama);

    printf("Gejala/Keluhan : ");
    fgets(nodeBaru->gejala, 100, stdin);
    hapusNewline(nodeBaru->gejala);

    printf("Kategori Triage:\n");
    printf("1. RED (Emergency - Kritis, Demam >40C, Kejang)\n");
    printf("2. YELLOW (Urgent - Luka Terbuka, Demam Tinggi)\n");
    printf("3. GREEN (Non-Urgent - Batuk, Pilek Ringan)\n");
    printf("Pilih prioritas (1/2/3): ");
    scanf("%d", &nodeBaru->triage);
    getchar(); // bersihkan buffer

    if (nodeBaru->triage < 1 || nodeBaru->triage > 3) {
        printf("Input tidak valid! Kategori diatur ke GREEN (3) secara default.\n");
        nodeBaru->triage = 3;
    }

    nodeBaru->next = NULL;
    masukkanKeAntrian(nodeBaru);
    printf("Pasien %s berhasil masuk ke antrian!\n", nodeBaru->nama);
}

// Fitur 2: Memanggil Pasien Terdepan (Dequeue)
void PanggilPasienNext() {
    if (head == NULL) {
        printf("\nAntrian kosong. Tidak ada pasien untuk dipanggil.\n");
        return;
    }

    Pasien* pasienDipanggil = head;
    head = head->next; // geser head ke node berikutnya

    printf("\n==========================================\n");
    printf("MEMANGGIL PASIEN KE RUANG DOKTER:\n");
    printf("Nama   : %s\n", pasienDipanggil->nama);
    printf("Gejala : %s\n", pasienDipanggil->gejala);
    printf("Triage : %d\n", pasienDipanggil->triage);
    printf("==========================================\n");

    free(pasienDipanggil); // bebaskan memori node yang dihapus
}

// Fitur 3: Menampilkan Seluruh Antrian Tersortir
void LihatDaftarAntrian() {
    if (head == NULL) {
        printf("\nAntrian saat ini kosong.\n");
        return;
    }

    printf("\n--- DAFTAR ANTRIAN KLINIK ---\n");
    printf("%-5s | %-20s | %-10s | %-30s\n", "No", "Nama", "Prioritas", "Gejala");
    printf("------------------------------------------------------------------------\n");

    Pasien* current = head;
    int nomor = 1;
    char warnaTriage[10];

    while (current != NULL) {
        if (current->triage == 1) strcpy(warnaTriage, "RED");
        else if (current->triage == 2) strcpy(warnaTriage, "YELLOW");
        else strcpy(warnaTriage, "GREEN");

        printf("%-5d | %-20s | %-10s | %-30s\n", nomor, current->nama, warnaTriage, current->gejala);
        current = current->next;
        nomor++;
    }
    printf("------------------------------------------------------------------------\n");
}

// Fitur 4: Pembaruan Kondisi Pasien (Eskalasi ke Kritis)
void UpdateKondisiPasien() {
    if (head == NULL) {
        printf("\nAntrian kosong.\n");
        return;
    }

    char namaDicari[50];
    printf("\nMasukkan nama pasien yang kondisinya memburuk: ");
    fgets(namaDicari, 50, stdin);
    hapusNewline(namaDicari);

    Pasien* current = head;
    Pasien* prev = NULL;