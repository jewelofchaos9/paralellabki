#include <omp.h> 
#include <stdio.h>

char* sched_type(int sched){
    switch(sched){
        case 0x1:
            return "static";
        case 0x2:
            return "dynamic";
        case 0x3:
            return "guided";
        case 0x4:
            return "auto";
        default:
            return "";
    }
}

int main(){
    printf("Version: %d\n", _OPENMP);
    printf("Date(m/y): %d/%d\n", _OPENMP % 100, _OPENMP / 100);
    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Number of max threads: %d\n", omp_get_max_threads());
    printf("Dynamic is enabled: %d\n", omp_get_dynamic());
    printf("Timer precision: %gs\n", omp_get_wtick());
    printf("Nested parallelism enabled: %d\n", omp_get_nested());     
    printf("Max number of nested active parallel regions: %d\n", omp_get_max_active_levels());
    omp_sched_t kind;
    int chunk_size;
    omp_get_schedule(&kind, &chunk_size);
    printf("Schedule kind is %s, chunk size is %d\n", sched_type((int)kind), chunk_size);
}
