#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

void isPrime(int n, int *counter) {
    int sqrtOfN = (int)sqrt(n);
    bool isPrime = true;

    if (n == 1)
        isPrime = false;

    for (int i = 2; i <= sqrtOfN; i++) {
        if (n % i == 0) {
            isPrime = false;
            break;
        }
    }

    if (isPrime) {
        *counter += 1;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Nstart> <Nend>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int Nstart = atoi(argv[1]);
    int Nend = atoi(argv[2]);

    clock_t start_time, end_time;
    double cpu_time_used;

    int prime_count = 0;

    start_time = clock();

    for (int num = Nstart; num <= Nend; num++) {
        isPrime(num, &prime_count);
    }

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Number of prime numbers between %d and %d: %d\n", Nstart, Nend, prime_count);
    printf("Time taken to execute: %f seconds\n", cpu_time_used);

    return 0;
}
