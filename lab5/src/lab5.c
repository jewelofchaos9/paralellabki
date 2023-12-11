#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char** argv)
{
	int ret  = -1;	///< For return values
	int size = -1;	///< Total number of processors
	int rank = -1;	///< This processor's number

	const int count = 100000000;; ///< Number of array elements

	int* array = 0; ///< The array we need to find the max in
	int lmax = -1;	///< Local maximums
	int  max = -1;  ///< The maximal element
  double start, end;

  const int random_seed = atoi(argv[1]); ///< RNG seed
	/* Initialize the MPI */
	ret = MPI_Init(&argc, &argv);

	/* Determine our rank and processor count */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Allocate the array */
  array = (int*)malloc(count * sizeof(int));
  if(!rank){
    srand(random_seed);
    for (int i = 0; i < count; i++) { array[i] = rand(); }
  }
    
  start = MPI_Wtime();
	MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

	const int wstart = (rank    ) * count / size;
	const int wend   = (rank + 1) * count / size;

	for (int i = wstart; i < wend; i++){
		if (array[i] > lmax){ 
            lmax = array[i]; 
      }
	}

	MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
	  end = MPI_Wtime();
	ret = MPI_Finalize();
  if (!rank) {
    printf("%g\n", end-start);
  }
	return(0);
}
