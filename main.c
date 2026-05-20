// Cari pasien berdasarkan nama
    while (current != NULL && strcmp(current->nama, namaDicari) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Pasien dengan nama %s tidak ditemukan dalam antrian.\n", namaDicari);
        return;
    }

    if (current->triage == 1) {
        printf("Pasien %s sudah berada di status RED (Prioritas 1).\n", current->nama);
        return;
    }

    // Cabut node dari posisinya saat ini di Linked List
    if (prev == NULL) {
        head = current->next; // Jika yang dicari ada di head
    } else {
        prev->next = current->next;
    }

    // Perbarui status menjadi RED dan sisipkan ulang ke dalam antrian
    printf("Kondisi %s memburuk! Status dinaikkan menjadi RED (Prioritas 1).\n", current->nama);
    current->triage = 1;
    current->next = NULL;
    
    masukkanKeAntrian(current); // Melompat ke depan sesuai prioritas baru
}

// Menu Utama
int main() {
    int pilihan;

    do {
        printf("\n=== SISTEM ANTRIAN KLINIK BERBASIS TRIAGE ===\n");
        printf("1. Registrasi Pasien (Enqueue)\n");
        printf("2. Panggil Pasien Berikutnya (Dequeue)\n");
        printf("3. Lihat Daftar Antrian\n");
        printf("4. Update Kondisi Pasien Darurat\n");
        printf("5. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar(); // Bersihkan buffer

        switch (pilihan) {
            case 1: RegistrasiPasien(); break;
            case 2: PanggilPasienNext(); break;
            case 3: LihatDaftarAntrian(); break;
            case 4: UpdateKondisiPasien(); break;
            case 5: 
                printf("Menutup sistem...\n");
                // Membersihkan sisa memori sebelum keluar
                while(head != NULL) {
                    Pasien* temp = head;
                    head = head->next;
                    free(temp);
                }
                break;
            default: printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 5);

    return 0;
}