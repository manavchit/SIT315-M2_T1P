#include <iostream>
#include <omp.h>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 500;
int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int N;

void generateMatrix(int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiplyMatrices() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(0));

    // Get matrix size from user
    do {
        cout << "Enter matrix size (max " << MAX_SIZE << "): ";
        cin >> N;
        if (N > MAX_SIZE || N <= 0) {
            cout << "Invalid size! Please enter a size between 1 and " << MAX_SIZE << ".\n";
        }
    } while (N > MAX_SIZE || N <= 0);

    generateMatrix(A);
    generateMatrix(B);

    // Set OpenMP to use the maximum available threads
    omp_set_num_threads(omp_get_max_threads());

    // Start measuring execution time
    auto start = high_resolution_clock::now();
    
    multiplyMatrices();

    auto stop = high_resolution_clock::now();

    cout << "Execution Time (OpenMP): " << duration<double>(stop - start).count() << " seconds" << endl;

    return 0;
}
