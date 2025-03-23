#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <sys/time.h> // For high-precision time measurement

void sieveOfEratosthenes(int N) {
    bool *isPrime = (bool *)malloc((N + 1) * sizeof(bool));
    for (int i = 0; i <= N; i++)
        isPrime[i] = true;

    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime

    for (int i = 2; i <= sqrt(N); i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }

    free(isPrime);
}

double get_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0); // Convert to seconds
}

int main() {
    int N = 10000; // Find primes up to this number
    int iterations = 10;
    double total_time = 0.0;

    for (int i = 0; i < iterations; i++) {
        double start = get_time_in_seconds();

        sieveOfEratosthenes(N);

        double end = get_time_in_seconds();
        double time_taken = end - start;
        total_time += time_taken;

        printf("Iteration %d Execution Time: %.6f s\n", i + 1, time_taken);
    }

    double avg_time = total_time / iterations;
    printf("\nAverage Execution Time over %d runs: %.6f s\n", iterations, avg_time);

    return 0;
}
