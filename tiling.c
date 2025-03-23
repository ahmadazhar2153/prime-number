#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 2000  // Max matrix size

// Function to initialize matrix
void initialize_matrix(double A[MAX_SIZE][MAX_SIZE], int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = (i + j) * 0.01;
}

// *Loop Interchange (Optimized for Cache Locality)*
void loop_interchange_c(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], int N) {
    double start = omp_get_wtime();
    
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < N; j++) {   // Interchanged loop order (column major)
        for (int i = 0; i < N; i++) {
            A[i][j] += B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Loop Interchange  coloum Time: %.6f sec\n", end - start);
}

// *Row-Major (Baseline for Comparison)*

void loop_interchange_r(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], int N) {
    double start = omp_get_wtime();
    
    #pragma omp parallel for 
    for (int i = 0; i < N; i++) {   // Interchanged loop order (row major)
        for (int j = 0; j < N; j++) {
            A[i][j] += B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Loop Interchange row Time: %.6f sec\n", end - start);
}

void loop_Tiling(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], int N, int tSize) {
    int tileSize = tSize;
    double start = omp_get_wtime();

    #pragma omp parallel for 
    for (int i = 0; i < N; i += tileSize) {
        for (int j = 0; j < N; j += tileSize) {
            for (int ii = i; ii < i + tileSize && ii < N; ii++) {
                for (int jj = j; jj < j + tileSize && jj < N; jj++) {
                    A[ii][jj] += B[ii][jj];
                }
            }
        }
    }

    double end = omp_get_wtime();
    printf("Loop tiling (%d) Time: %.6f sec\n",tileSize, end - start);
}

int main() {
        omp_set_num_threads(8);
        printf("\n--- Testing with 8 Threads ---\n");

    
            int N = 256;
            printf("\nMatrix Size: %d x %d\n", N, N);

            static double A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE];
            initialize_matrix(A, N);
            initialize_matrix(B, N);

            
            loop_interchange_c(A, B, N);
            loop_interchange_r(A, B, N);
            loop_Tiling(A,B,N,8);
            loop_Tiling(A,B,N,16);
            loop_Tiling(A,B,N,32);
            loop_Tiling(A,B,N,64);
    return 0;
}