/*
================================================================================
   STUDI KASUS: ARTIFICIAL INTELLIGENCE DALAM ASISTEN KESEHATAN
   Arsitektur Dinamis: Dynamic Array, Dynamic Queue, Dynamic Stack, 
   Linked List, Tree, Graph, BFS, DFS, Sorting, Searching, Big O Notation.
================================================================================
*/

#include<iostream>
#include<string>
#include<iomanip>
#include<utility>
#include<limits>    //untuk buffer input

using namespace std;

// TIPE DATA & STRUCT DASAR
typedef int PatientID;
typedef double Temperature;
typedef int RiskScore;

const double FEVER_THRESHOLD = 37.5;

struct PatientRecord
{
    PatientID patientId;
    string patientName;
    int age;
    Temperature bodyTemperature;
    string symptoms;
    RiskScore riskScore;
    string diagnosis;
    /* data */
};

// Mencegah buffer lompat saat spasi
string safeGetline(const string& prompt) {
    cout << prompt;
    string res;
    getline(cin >> ws, res);
    return res;
}

// ============================================================
// SUB 1: DYNAMIC ARRAY — RIWAYAT REKAM MEDIS (TIDAK ADA DATA HILANG)
// ============================================================
class DiagnosisHistory {
    public:
    PatientRecord* records;
    int capacity;
    int count;

    DiagnosisHistory() {
        capacity = 5; // Kapasitas awal, otomatis membesar
        count = 0;
        records = new PatientRecord[capacity];
    }
    ~DiagnosisHistory() {
        delete[] records; // Mencegah memory leak
    }

    // Fungsi memperbesar ukuran array jika penuh
    void resizeArray() {
        capacity *= 2; // Lipat ganda kapasitas
        PatientRecord* newRecords = new PatientRecord[capacity];
        for (int i = 0; i < count; i++) {
            newRecords[i] = records[i];
        }
        delete[] records;
        records = newRecords;
    }

    void addRecord(const PatientRecord& rec) {
        if (count >= capacity){
            resizeArray(); // Data lama tidak ditimpa
        }
        records[count++] = rec;
    }
    
    void printHistory() const {
        if (count == 0) {
            cout << "BELUM ADA REKAM MEDIS YANG TERSIMPAN.\n";
            return;
        }
        cout << "\n+----+-------+----------------------+-----+--------+----------+--------------------------------+\n";
        cout << "| No | ID    | Nama Pasien          | Usia| Suhu°C | Risiko   | Diagnosa AI                    |\n";
        cout << "+----+-------+----------------------+-----+--------+----------+--------------------------------+\n";
        for (int i = 0; i < count; i++) {
            const PatientRecord& r = records[i];
            cout << "| " << setw(2) << (i + 1)
                 << " | " << setw(5) << r.patientId
                 << " | " << setw(20) << left << r.patientName.substr(0, 20)
                 << " | " << setw(3) << right << r.age
                 << " | " << setw(6) << fixed << setprecision(1) << r.bodyTemperature
                 << " | " << setw(8) << r.riskScore
                 << " | " << setw(30) << left << r.diagnosis.substr(0, 30) << " |\n";
        }
        cout << "+----+-------+----------------------+-----+--------+----------+--------------------------------+\n";
    }
};

// ============================================================
// SUB 2: DYNAMIC QUEUE — ANTREAN PASIEN UNBOUNDED
// ============================================================
class AIHealthQueue {
private:
    PatientRecord* queueArray;
    int front;
    int rear;
    int capacity;
    int currentSize;

    void resizeQueue() {
        int newCapacity = capacity * 2;
        PatientRecord* newArray = new PatientRecord[newCapacity];
        // Susun ulang queue melingkar menjadi lurus
        for (int i = 0; i < currentSize; i++) {
            newArray[i] = queueArray[(front + i) % capacity];
        }
        delete[] queueArray;
        queueArray = newArray;
        front = 0;
        rear = currentSize - 1;
        capacity = newCapacity;
    }

public:
    AIHealthQueue() {
        capacity = 3; // Kapasitas awal kecil, tapi dinamis membesar
        queueArray = new PatientRecord[capacity];
        front = 0;
        rear = -1;
        currentSize = 0;
    }

    ~AIHealthQueue() { delete[] queueArray; }

    bool isEmpty() const { return (currentSize == 0); }
    int  getCount() const { return currentSize; }

    void enqueue(PatientRecord patient) {
        if (currentSize == capacity) resizeQueue(); // Pastikan pasien tidak pernah ditolak
        rear = (rear + 1) % capacity;
        queueArray[rear] = patient;
        currentSize++;
        cout << "[ANTREAN] Pasien " << patient.patientName << " sukses masuk ke buffer.\n";
    }

    PatientRecord dequeue() {
        if (isEmpty()) {
            return {0, "", 0, 0.0, "", 0, ""};
        }
        PatientRecord target = queueArray[front];
        front = (front + 1) % capacity;
        currentSize--;
        return target;
    }
};

// ============================================================
// SUB 3: DYNAMIC STACK — LOG SISTEM (LIFO)
// ============================================================
struct ActionLog {
    string action;
    string detail;
};

class AIActionStack {
private:
    ActionLog* logs;
    int topIndex;
    int capacity;

    void resizeStack() {
        capacity *= 2;
        ActionLog* newLogs = new ActionLog[capacity];
        for (int i = 0; i <= topIndex; i++) newLogs[i] = logs[i];
        delete[] logs;
        logs = newLogs;
    }

public:
    AIActionStack() {
        capacity = 5;
        logs = new ActionLog[capacity];
        topIndex = -1;
    }
    ~AIActionStack() { delete[] logs; }

    bool isEmpty() const { return (topIndex == -1); }

    void push(const string& act, const string& det) {
        if (topIndex == capacity - 1) resizeStack(); // Log historis aman
        logs[++topIndex] = {act, det};
    }

    void printStack() const {
        cout << "\n  === AUDIT LOG SISTEM (Dari Terbaru) ===\n";
        if (isEmpty()) { cout << "  [Kosong]\n"; return; }
        for (int i = topIndex; i >= 0; i--) {
            cout << "  -> [" << logs[i].action << "] " << logs[i].detail << "\n";
        }
    }
};

// ============================================================
// SUB 4: LINKED LIST — MANAJEMEN KONDISI MEDIS
// ============================================================
struct ConditionNode {
    string conditionName;
    int severityLevel;
    ConditionNode* next;
};

class MedicalConditionList {
private:
    ConditionNode* head;
public:
    MedicalConditionList() : head(nullptr) {}
    ~MedicalConditionList() {
        while (head) {
            ConditionNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insertBack(const string& name, int severity) {
        ConditionNode* newNode = new ConditionNode{name, severity, nullptr};
        if (!head) { head = newNode; }
        else {
            ConditionNode* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = newNode;
        }
        cout << "[LINKED-LIST] Kondisi \"" << name << "\" ditambahkan.\n";
    }

    void printList() const {
        cout << "\n  === DATABASE KONDISI MEDIS BISA DITANGANI ===\n";
        if (!head) { cout << "  [Data Kosong]\n"; return; }
        ConditionNode* cur = head;
        int idx = 1;
        const char* levels[] = {"", "RINGAN", "SEDANG", "BERAT"};
        int safeLevel = (cur->severityLevel >= 1 && cur->severityLevel <= 3) ? cur->severityLevel : 0;
        while (cur) {
            cout << "  " << idx++ << ". " << left << setw(25) << cur->conditionName 
                 << " [Level: " << levels[cur->severityLevel] << "]\n";
            cur = cur->next;
        }
    }
};

// ============================================================
// SUB 5: DECISION TREE — POHON DIAGNOSA
// ============================================================
struct TreeNode {
    string question;
    string diagnosis;
    bool isLeaf;
    TreeNode* leftChild;
    TreeNode* rightChild;

    // Node Daun (Hasil)
    TreeNode(const string& diag) : question(""), diagnosis(diag), isLeaf(true), leftChild(nullptr), rightChild(nullptr) {}
    // Node Cabang (Pertanyaan)
    TreeNode(const string& q, TreeNode* l, TreeNode* r) : question(q), diagnosis(""), isLeaf(false), leftChild(l), rightChild(r) {}
};

class DiagnosisTree {
private:
    TreeNode* root;
    void destroyTree(TreeNode* node) {
        if (!node) return;
        destroyTree(node->leftChild);
        destroyTree(node->rightChild);
        delete node;
    }
public:
    DiagnosisTree() {
        // Pembuatan pohon statis untuk contoh klasifikasi
        TreeNode* demamBiasa = new TreeNode("Demam Biasa - Istirahat");
        TreeNode* infeksiBakt = new TreeNode("Suspek Infeksi Bakteri");
        TreeNode* alergi = new TreeNode("Alergi Ringan");
        TreeNode* tipes = new TreeNode("Suspek Tipes - Cek Lab");
        TreeNode* sehat = new TreeNode("Kondisi Normal");
        TreeNode* hipotermia = new TreeNode("Hipotermia");
        TreeNode* kelelahan = new TreeNode("Kelelahan Fisik");

        TreeNode* checkDurasi = new TreeNode("Gejala > 3 hari?", tipes, infeksiBakt);
        TreeNode* checkBatuk = new TreeNode("Disertai batuk/pilek?", alergi, demamBiasa);
        TreeNode* checkAktivitas = new TreeNode("Baru aktivitas berat?", kelelahan, hipotermia);
        TreeNode* checkTinggi = new TreeNode("Suhu > 38.5C?", checkDurasi, checkBatuk);

        root = new TreeNode("Suhu >= 37.5C?", checkTinggi, new TreeNode("Suhu < 36.0C?", checkAktivitas, sehat));
    }
    ~DiagnosisTree() { destroyTree(root); }

    RiskScore calculateRisk(const PatientRecord& p) const {
        RiskScore score = 0;
        if (p.bodyTemperature >= 39.0) score += 40;
        else if (p.bodyTemperature >= 37.5) score += 20;
        
        if (p.age > 60 || p.age < 5) score += 30;
        
        if (p.symptoms.find("lama") != string::npos) score += 20;
        return (score > 100) ? 100 : score;
    }

    string diagnose(const PatientRecord& p) const {
        TreeNode* cur = root;
        while (cur && !cur->isLeaf) {
            bool goLeft = false;
            // Evaluasi Text-Parsing sederhana untuk traversing
            if (cur->question.find("37.5") != string::npos) goLeft = (p.bodyTemperature >= 37.5);
            else if (cur->question.find("38.5") != string::npos) goLeft = (p.bodyTemperature > 38.5);
            else if (cur->question.find("3 hari") != string::npos) goLeft = (p.symptoms.find("lama") != string::npos);
            else if (cur->question.find("batuk") != string::npos) goLeft = (p.symptoms.find("batuk") != string::npos);
            else if (cur->question.find("36.0") != string::npos) goLeft = (p.bodyTemperature < 36.0);
            else if (cur->question.find("berat") != string::npos) goLeft = (p.symptoms.find("lelah") != string::npos);
            
            cur = goLeft ? cur->leftChild : cur->rightChild;
        }
        return cur ? cur->diagnosis : "Tidak Terdiagnosa";
    }
};

// ============================================================
// SUB 6: GRAPH (ADJACENCY LIST) — JARINGAN RUJUKAN
// ============================================================
const int MAX_RS = 5;
struct GraphNode { int tujuan, jarak; GraphNode* next; };

class RujukanGraph {
private:
    GraphNode* adjList[MAX_RS];
    string namaRS[MAX_RS] = {"Klinik", "RSUD", "RS Jantung", "Puskesmas", "RS Infeksi",};
public:
    RujukanGraph() {
        for (int i = 0; i < MAX_RS; i++) adjList[i] = nullptr;
        tambahEdge(0, 1, 5); tambahEdge(0, 3, 2);
        tambahEdge(1, 2, 8); tambahEdge(1, 4, 6);
        tambahEdge(3, 4, 7);
    }
    ~RujukanGraph() {
        for (int i = 0; i < MAX_RS; i++) {
            while (adjList[i]) {
                GraphNode* tmp = adjList[i];
                adjList[i] = adjList[i]->next;
                delete tmp;
            }
        }
    }
    void tambahEdge(int dari, int ke, int jarak) {
        adjList[dari] = new GraphNode{ke, jarak, adjList[dari]};
        adjList[ke] = new GraphNode{dari, jarak, adjList[ke]};
    }
    void printGraph() const {
        cout << "\n  === JARINGAN RUJUKAN RUMAH SAKIT ===\n";
        for (int i = 0; i < MAX_RS; i++) {
            cout << "  [" << i << "] " << left << setw(15) << namaRS[i] << " Terhubung ke: ";
            GraphNode* cur = adjList[i];
            while (cur) { cout << namaRS[cur->tujuan] << "(" << cur->jarak << "km) "; cur = cur->next; }
            cout << "\n";
        }
    }
// ============================================================
// SUB 7: HELPER FUNGSI REKURSIF UNTUK DFS
// ============================================================
    void DFSRecursive(int current, bool visited[]) {
        visited[current] = true;
        cout << "  -> " << namaRS[current] << "\n";

        GraphNode* cur = adjList[current];
        while (cur) {
            if (!visited[cur->tujuan]) {
                DFSRecursive(cur->tujuan, visited);
            }
            cur = cur->next;
        }
    }

    // DFS (Depth First Search)
    void DFS(int startNode) {
        cout << "\n  [DFS] Menelusuri jalur rujukan hingga ujung (Mendalam)...\n";
        bool visited[MAX_RS] = {false};
        DFSRecursive(startNode, visited);
    }

// ============================================================
// SUB 8: BFS (BREADTH FIRST SEARCH)
// ============================================================
    void BFS(int startNode) {
        cout << "\n  [BFS] Mencari rujukan terdekat secara berlapis (Melebar)...\n";
        bool visited[MAX_RS] = {false};
        int queue[MAX_RS];
        int front = 0, rear = 0;

        visited[startNode] = true;
        queue[rear++] = startNode;

        while (front < rear) {
            int current = queue[front++];
            cout << "  -> " << namaRS[current] << "\n";

            GraphNode* cur = adjList[current];
            while (cur) {
                if (!visited[cur->tujuan]) {
                    visited[cur->tujuan] = true;
                    queue[rear++] = cur->tujuan;
                }
                cur = cur->next;
            }
        }
    }
};

// ============================================================
// SUB 9: SEARCHING & SORTING
// ============================================================
void bubbleSortByRisk(PatientRecord arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].riskScore < arr[j + 1].riskScore) { // Descending (Risiko tertinggi di atas)
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

int binarySearchByID(PatientRecord arr[], int n, int targetID) {
    // Note: Pastikan array di-sort by ID sebelum panggil ini. 
    // Untuk penyederhanaan, kita gunakan Binary Search dengan asumsi data telah di-sort by ID.
    int kiri = 0, kanan = n - 1;
    while (kiri <= kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        if (arr[tengah].patientId == targetID) return tengah;
        if (arr[tengah].patientId < targetID) kiri = tengah + 1;
        else kanan = tengah - 1;
    }
    return -1;
}

// ============================================================
// SUB 10: BIG O NOTATION SUMMARY
// ============================================================
void printBigOSummary() {
    cout << "\n======================================================\n";
    cout << "  RINGKASAN BIG O NOTATION SISTEM AI KESEHATAN\n";
    cout << "======================================================\n";
    cout << "  Struktur Data / Algoritma      | Kompleksitas Waktu\n";
    cout << "  -------------------------------|--------------------\n";
    cout << "  Array (Dynamic) - Akses/Insert | O(1) Amortized\n";
    cout << "  Queue (Dynamic) - Enq/Dequeue  | O(1)\n";
    cout << "  Stack (Dynamic) - Push/Pop     | O(1)\n";
    cout << "  Linked List - Insert Back      | O(n)\n";
    cout << "  Binary Tree - Traversal Diagnos| O(log n) s.d O(n)\n";
    cout << "  Graph - BFS / DFS              | O(V + E)\n";
    cout << "  Bubble Sort                    | O(n^2)\n";
    cout << "  Binary Search                  | O(log n)\n";
    cout << "======================================================\n";
}

// ============================================================
// FUNGSI MENU & MAIN
// ============================================================
void printMenu() {
    cout << "\n======================================================\n";
    cout << "     ALUR KERJA KLINIK AI (HUMAN WORKFLOW MENU)\n";
    cout << "======================================================\n";
    cout << " [ FASE 1: PENDAFTARAN & ADMINISTRASI ]\n";
    cout << "   1. Pendaftaran Pasien Baru (Masuk Antrean)\n";
    cout << "   2. Manajemen Kondisi Medis Sistem\n";
    cout << " [ FASE 2: PEMERIKSAAN AI ]\n";
    cout << "   3. Proses Diagnosa Pasien (Tarik Antrean)\n";
    cout << " [ FASE 3: REKAM MEDIS & PENCARIAN ]\n";
    cout << "   4. Lihat Seluruh Riwayat Rekam Medis\n";
    cout << "   5. Urutkan Pasien Berdasarkan Risiko Terparah\n";
    cout << " [ FASE 4: SISTEM RUJUKAN & AUDIT ]\n";
    cout << "   6. Lihat Jaringan Rujukan RS Terdekat\n";
    cout << "   7. Lihat Log Aktivitas Sistem\n";
    cout << "   8. Jalankan Penelusuran Mendalam (DFS) Jaringan RS\n";
    cout << "   9. Jalankan Penelusuran Melebar (BFS) Jaringan RS\n";
    cout << "  10. Tampilkan Evaluasi Big O Notation\n";
    cout << "   0. Keluar Sistem\n";
    cout << "======================================================\n";
    cout << "Pilih langkah (0-10): ";
}

int main() {
    AIHealthQueue        aiBuffer;
    AIActionStack        actionLog;
    DiagnosisHistory     history;
    DiagnosisTree        diagTree;
    MedicalConditionList conditionList;
    RujukanGraph         rsGraph;

    // Kondisi Default untuk contoh
    conditionList.insertBack("Demam Biasa", 1);
    conditionList.insertBack("Tipes", 2);
    conditionList.insertBack("Infeksi Bakteri", 3);
    conditionList.insertBack("Hipotermia", 3);
    conditionList.insertBack("Kelelahan", 2);
    conditionList.insertBack("Pusing", 1);

    int pilihan = -1;
    while (pilihan != 0) {
        printMenu();
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (pilihan) {
            case 1: {
                cout << "\n--- PENDAFTARAN PASIEN ---\n";
                PatientRecord p;
                p.riskScore = 0; p.diagnosis = "";
                cout << "  ID Pasien       : "; cin >> p.patientId;
                p.patientName = safeGetline("  Nama Lengkap    : ");
                cout << "  Usia (tahun)    : "; cin >> p.age;
                cout << "  Suhu Tubuh (C)  : "; cin >> p.bodyTemperature;
                p.symptoms = safeGetline("  Gejala Dominan  : ");
                
                aiBuffer.enqueue(p);
                actionLog.push("ENQUEUE", "Pendaftaran ID: " + to_string(p.patientId));
                break;
            }
            case 2: {
                conditionList.printList();
                actionLog.push("VIEW", "Melihat Master Data Kondisi");
                break;
            }
            case 3: {
                cout << "\n--- PEMERIKSAAN AI ---\n";
                if (aiBuffer.isEmpty()) {
                    cout << "  [INFO] Ruang tunggu kosong. Tidak ada pasien.\n";
                } else {
                    PatientRecord job = aiBuffer.dequeue();
                    job.riskScore = diagTree.calculateRisk(job);
                    job.diagnosis = diagTree.diagnose(job);
                    history.addRecord(job); // SIMPAN KE REKAM MEDIS PERMANEN
                    
                    cout << "  [BERHASIL] Pasien " << job.patientName << " selesai dianalisis.\n";
                    cout << "  -> Diagnosa : " << job.diagnosis << "\n";
                    cout << "  -> Risiko   : " << job.riskScore << "/100\n";
                    actionLog.push("DIAGNOSA", "Pasien: " + job.patientName);
                }
                break;
            }
            case 4: {
                history.printHistory();
                actionLog.push("VIEW", "Membuka Riwayat Rekam Medis");
                break;
            }
            case 5: {
                if (history.count < 2) {
                    cout << "  [INFO] Data belum cukup untuk diurutkan.\n";
                } else {
                    bubbleSortByRisk(history.records, history.count);
                    cout << "  [BERHASIL] Data diurutkan dari Risiko Tertinggi.\n"; // (Worst Case O(n^2))
                    history.printHistory();
                    actionLog.push("SORT", "Urut Data by Risiko");
                }
                break;
            }
            case 6: {
                rsGraph.printGraph();
                actionLog.push("VIEW", "Melihat Jaringan RS");
                break;
            }
            case 7: {
                actionLog.printStack();
                break;
            }
            case 8: {
                cout << "\n--- PENELUSURAN MENDALAM JARINGAN RS (DFS) ---\n";
                // Secara default kita mulai penelusuran dari node 0 (Klinik kita sendiri)
                int startNode = 0; 
                cout << "  Dimulai dari node [" << startNode << "]...\n";
                
                // Memanggil algoritma Depth First Search
                rsGraph.DFS(startNode);
                
                // Mencatat ke dalam stack log aktivitas
                actionLog.push("GRAPH", "Jalankan Algoritma DFS dari Node 0");
                break;
            }
            case 9: {
                cout << "\n--- PENELUSURAN MELEBAR JARINGAN RS (BFS) ---\n";
                int startNode = 0; 
                cout << "  Dimulai dari node [" << startNode << "]...\n";
                
                rsGraph.BFS(startNode);
                actionLog.push("GRAPH", "Jalankan Algoritma BFS dari Node 0");
                break;
            }
            case 10: {
                printBigOSummary();
                actionLog.push("VIEW", "Melihat Analisis Big O Notation");
                break;
            }
            case 0: {
                cout << "Menutup Sistem AI dan Membersihkan Memori (Destructor dipanggil)... Selesai.\n";
                break;
            }
            default: cout << "[!] Pilihan tidak valid.\n";
        }
    }
    return 0;
}