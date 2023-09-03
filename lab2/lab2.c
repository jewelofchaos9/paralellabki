#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>
long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

long long main(int argc, char** argv)
{
  const int count = 100000000;     ///< Number of array elements
  const int target = 16;          ///< Number to look for

  int* array = 0;                 ///< The array we need to find the max in
  int  index = -1;                ///< The index of the element we need
  if (argc < 3){
    puts("USAGE ./a.out {THREADS_NUM} {SEED}");
    return -1;
  }
  const int threads = atoi(argv[1]);         ///< Number of parallel threads to use
  const int random_seed = atoi(argv[2]); ///< RNG seed

  /* Initialize the RNG */
  srand(random_seed);

  /* Generate the random array */

  /*
   * We can multithread array filling
   * */

  array = (int*)malloc(count*sizeof(int));
  #pragma omp parallel num_threads(threads) shared(array, count)
  { 
    #pragma omp for
    for(int i=0; i<count; i++){ 
      array[i] = rand(); 
    }
  }

  /* Find the index of the element */
  /*
   * We can multithread index finding
   * */
  

  long long time_start = timeInMilliseconds();
  #pragma omp parallel num_threads(threads) shared(array, count, target) reduction(max: index) default(none)
  {
    #pragma omp for
    for(int i=0; i<count; i++)
    {   
        if(array[i] == target)
        {
            index = i;
        }
    }
  }
  free(array);
  if(index != -1){
    printf("Found occurence of %d at index %d;\n", target, index);
  }
  return timeInMilliseconds() - time_start;
}
