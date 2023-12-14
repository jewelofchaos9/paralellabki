#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

const int ROOT_PROCESS_ID = 0;

void isPrime(int n, int threads, int *counter) {
  int sqrtOfN = (int) sqrt(n);

  bool isPrime = true;

  if (n == 1) isPrime = false;

  #pragma omp parallel shared(isPrime, sqrtOfN)
  {
    #pragma omp for
    for (int i = 2; i <= sqrtOfN; i++) {
      if (n % i == 0) {
        isPrime = false;
      }
      if (!isPrime) {
        i = sqrtOfN;
      }
    }
    if (isPrime) {
      #pragma omp critical
      {
        *counter += 1;
      }
    }
  }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double startTime, endTime;
    
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 3) {
        if (rank == ROOT_PROCESS_ID) {
            fprintf(stderr, "Usage: %s <Nstart> <Nend>\n", argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    int Nstart = atoi(argv[1]);
    int Nend = atoi(argv[2]);

    // Calculate workload for each process
    int workload = (Nend - Nstart + 1) / size;
    int remainder = (Nend - Nstart + 1) % size;

    // Distribute workload to processes
    int destStart = Nstart + rank * workload + (rank < remainder ? rank : remainder);
    int destEnd = destStart + workload - 1 + (rank < remainder ? 1 : 0);  

    // Set the number of threads for OpenMP
    int maxThreads = omp_get_max_threads();
    int numThreads = (size > maxThreads) ? maxThreads / size : 2;

    omp_set_num_threads(numThreads);

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();

    // Process the workload for each process
    int prime_count = 0;
    #pragma omp parallel for
    for (int num = destStart; num <= destEnd; num++) {
      isPrime(num, numThreads, &prime_count);
    }

    endTime = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    double elapsedTime = endTime - startTime;

    // Collect results from other processes
    double* allElapsedTimes = NULL;
    if (rank == ROOT_PROCESS_ID) {
        allElapsedTimes = (double*)malloc(size * sizeof(double));
    }

    MPI_Gather(&elapsedTime, 1, MPI_DOUBLE, allElapsedTimes, 1, MPI_DOUBLE, ROOT_PROCESS_ID, MPI_COMM_WORLD);

    int* allPrimeCounts = NULL;
    if (rank == ROOT_PROCESS_ID) {
        allPrimeCounts = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(&prime_count, 1, MPI_INT, allPrimeCounts, 1, MPI_INT, ROOT_PROCESS_ID, MPI_COMM_WORLD);

    if (rank == ROOT_PROCESS_ID) {
	printf("%g\n", elapsedTime);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
