#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;

int main(){
  int n;
  omp_init_lock(&lock);
  int g = 0;
  int threads = 5000;
  #pragma omp parallel for private(n) shared(g) num_threads(threads)
  for(int i = 0; i < threads; i++){
      n = omp_get_thread_num();
      g += n;
  }
  omp_destroy_lock(&lock);
  printf("%d\n", g);
}
