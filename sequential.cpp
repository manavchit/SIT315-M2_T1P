
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace chrono;

const int MAX_SIZE = 500; // Upper limit for matrix size
int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE]; 
int N; // User-defined size

// Function to generate random matrices
void generateMatrix(int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

// Sequential matrix multiplication
void multiplyMatrices() {
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

    // Get user input for matrix size
    do {
        cout << "Enter matrix size (max " << MAX_SIZE << "): ";
        cin >> N;
        if (N > MAX_SIZE || N <= 0) {
            cout << "Invalid size! Please enter a size between 1 and " << MAX_SIZE << ".\n";
        }
    } while (N > MAX_SIZE || N <= 0);

    generateMatrix(A);
    generateMatrix(B);

    auto start = high_resolution_clock::now();
    multiplyMatrices();
    auto stop = high_resolution_clock::now();
    
    cout << "Execution Time (Sequential): " << duration<double>(stop - start).count() << " seconds" << endl;
    return 0;
}
