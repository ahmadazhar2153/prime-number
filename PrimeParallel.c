#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>

// Function to compute prime numbers using the Sieve of Eratosthenes
void sieveOfEratosthenes(int N, int num_threads) {
    bool *isPrime = (bool *)malloc((N + 1) * sizeof(bool));
    
    if (isPrime == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Initialize all values to true (assuming all numbers are prime)
    #pragma omp parallel for schedule(static) num_threads(num_threads)
    for (int i = 0; i <= N; i++)
        isPrime[i] = true;

    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime

    int sqrtN = (int)sqrt(N);  // Convert sqrt(N) to an integer

    #pragma omp parallel for schedule(static) num_threads(num_threads)
    for (int i = 2; i <= sqrtN; i++) {
        if (isPrime[i]) {
            // Use OpenMP parallelization for marking non-prime numbers
            #pragma omp parallel for schedule(static) num_threads(num_threads)
            for (int j = i * i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // Print the prime numbers
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
    if (scanf("%d", &num_threads) != 1 || num_threads <= 0) {
        printf("Invalid input! Please enter a positive integer.\n");
        return 1;
    }

    double start_time = omp_get_wtime();
    sieveOfEratosthenes(N, num_threads);
    double end_time = omp_get_wtime();

    printf("\nExecution Time: %.6f seconds with %d threads\n", (end_time - start_time), num_threads);

    return 0;
}
