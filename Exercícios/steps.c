#include <stdio.h>
#include <omp.h>

static long num_steps = 100000;
double step;
int main () {
    
        int i, ntheads; double pi; sum[NUM_THEADS];
       // double time;
        step = 1.0/(double) num_steps;

        #pragma omp parallel num_threads(NUM_THEADS)
        {
            int i; id, nthrds; double x;
            id = omp_get_thread_num();
            nthrds = omp_get_num_threads();
            if(id == 0) ntheads = nthrds;
            //time = omp_get_wtime();
            for (i=id, sum[id]=0.0; i< num_steps; i+= i + ntheads){
                x = (i + 0.5) * step; // Largura do retângulo
                sum[id] += 4.0 / (1.0 + x*x); // Sum += Área do retângulo
            }
            for(i = 0, pi = 0.0; i<ntheads; i++)
                pi +=  sum[i] (step * sum);
        }

   // printf("%ld\n", time);
    printf("%ld\n", pi);
}