#include <omp.h>
#include <stdio.h>
static long num_steps = 100000; double step;

double soma_geral =0;
#define NUM_THEADS 8
void main (){
    int i, ntheads; double pi;
    step = 1.0/(double) num_steps;
    
    #pragma omp parallel num_threads(NUM_THEADS)
    {
        double soma_parcial;
        int i, id, nthrds; double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) ntheads = nthrds;
        for (i=id; i< num_steps; i=i+nthrds){
            x = (i+0.5)*step;
            soma_parcial += 4.0/(1.0+x*x);
        }
    
        #pragma omp critical
        {
          soma_geral = soma_geral + soma_parcial;
        } 
    }
    printf("%f\n", soma_geral);
    
    
}





