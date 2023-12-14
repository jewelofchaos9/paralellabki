#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void new_array(int*, unsigned int*, int);
void shellsort(int*, int);

void new_array(int* array, unsigned int* random_seed, int count){
    int i;
    for(i=0; i < count; i++) { 
      array[i] = rand(); 
    }
    *random_seed += rand();
}

void shellsort(int* array, int count){
    for(int gap = count/2; gap > 0; gap/=2){
        for(int i = gap; i < count; i++){
            for(int j=i; j>=gap  && array[j-gap] > array[j]; j-=gap){
                int tmp = array[j];
                array[j] = array[j-gap];
                array[j-gap] = tmp;
            }
        }
    }
}

int main(int argc, char** argv){
    const int count = 1000000;
    long seed = atoi(argv[1]);
    srand(seed);
    unsigned int random_seed = 1337;
    double t1, t2;

    int ret = MPI_Init(&argc, &argv);
    int size, rank;
    int* array = (int*)calloc(count, sizeof(int));

    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        new_array(array, &random_seed, count);
    }

    int n = count / size;

    if(rank == 0){
        t1 = MPI_Wtime();
        for(int i = 1; i < size; i++){ // send each part, except for last one
            MPI_Send(array + n * (i - 1), n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        int k = count - n * (size - 1); // main one
        shellsort(array + n * (size - 1), k);

        for(int i = 0; i < size - 1; i++){
            MPI_Recv(array + n * i, n, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        }

        shellsort(array, count); // final one
        t2 = MPI_Wtime();
        
        fprintf(stdout, "%g",t2 - t1);
        free(array);
    }
    else{
        int* cur = (int*)calloc(n, sizeof(int));
        MPI_Recv(cur, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        shellsort(cur, n);
        MPI_Send(cur, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
