#include <stdio.h>
#include <omp.h>

int main(){
    #pragma omp parallel num_threads(1)
        printf("Sou a thread = %i ", omp_get_thread_num());
}