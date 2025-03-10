
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 500;  
const int MAX_THREADS = 8;  // Maximum number of threads
int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];  
int N, NUM_THREADS;

struct ThreadData {
    int thread_id;
};

// Function to generate random matrix values
void generateMatrix(int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;  // Values between 0-9
        }
    }
}

// Function to write the output matrix to a file
void writeMatrixToFile(const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file << C[i][j] << " ";
        }
        file << "\n";
    }
    file.close();
}

// Parallel matrix multiplication function
void* multiplyPart(void* arg) {
    auto* data = static_cast<ThreadData*>(arg);
    int start = (data->thread_id * N) / NUM_THREADS;
    int end = ((data->thread_id + 1) * N) / NUM_THREADS;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return nullptr;
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Seed random values

    // Get matrix size and number of threads
    do {
        cout << "Enter matrix size (max " << MAX_SIZE << "): ";
        cin >> N;
        if (N > MAX_SIZE || N <= 0) {
            cout << "Invalid size! Please enter a size between 1 and " << MAX_SIZE << ".\n";
        }
    } while (N > MAX_SIZE || N <= 0);

    do {
        cout << "Enter number of threads (max " << MAX_THREADS << "): ";
        cin >> NUM_THREADS;
        if (NUM_THREADS > MAX_THREADS || NUM_THREADS <= 0) {
            cout << "Invalid thread count! Please enter between 1 and " << MAX_THREADS << ".\n";
        }
    } while (NUM_THREADS > MAX_THREADS || NUM_THREADS <= 0);

    // Generate matrices
    generateMatrix(A);
    generateMatrix(B);

    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];

    auto start = high_resolution_clock::now();  // Start time

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].thread_id = i;
        pthread_create(&threads[i], nullptr, multiplyPart, &threadData[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto stop = high_resolution_clock::now();  // Stop time

    cout << "Execution Time (Pthreads): " << duration<double>(stop - start).count() << " seconds" << endl;

    // Write result to file
    writeMatrixToFile("output.txt");

    return 0;
}
