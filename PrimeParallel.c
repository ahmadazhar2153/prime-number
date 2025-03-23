#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>

// Function to find prime numbers using the Sieve of Eratosthenes (Dynamic Scheduling)
void sieveOfEratosthenes(int N, int num_threads) {
    bool *isPrime = (bool *)malloc((N + 1) * sizeof(bool));

    // Parallel initialization of isPrime array using static scheduling
    #pragma omp parallel for schedule(static)
    for (int i = 0; i <= N; i++)
        isPrime[i] = true;

    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime

    omp_set_num_threads(num_threads); // Set the number of threads

    int sqrt_N = (int)sqrt(N); // Precompute square root of N to avoid redundant calculations

    // Parallel loop with dynamic scheduling (Chunk size = 3000)
    #pragma omp parallel for schedule(dynamic, 3000)
    for (int i = 2; i <= sqrt_N; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // Barrier to ensure all threads finish before printing
    #pragma omp barrier

    printf("Prime numbers up to %d:\n", N);
    for (int i = 2; i <= N; i++) {
        if (isPrime[i])
            printf("%d ", i);
    }
    printf("\n");

    free(isPrime);
}

int main() {
    int N = 10000; // Upper limit for prime numbers
    int num_threads;

    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    double start_time = omp_get_wtime();
    sieveOfEratosthenes(N, num_threads);
    double end_time = omp_get_wtime();

    printf("\nExecution Time: %.3f seconds with %d threads and chunk size 3000 (Dynamic Scheduling)\n", 
           (end_time - start_time), num_threads);
    
    return 0;
}
