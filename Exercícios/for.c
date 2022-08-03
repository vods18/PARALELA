#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

static long num_steps = 100000;
double step;
double soma_geral =0.0;
#define NUM_THEADS 2

double timestamp(){
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

int main () {
        
    int ntheads; double pi, sum=0.0;
    step = 1.0/(double) num_steps;
    double time = timestamp();

    #pragma omp parallel 
    { 

        int i; 
        double x;
        
        #pragma omp for reduction (+:sum) private(x)
        for (i=0;i< num_steps; i++){
            x = (i + 0.5) * step; // Largura do retângulo
        
            sum += 4.0 / (1.0 + x*x); // Sum += Área do retângulo
        }
    }

    sum *= step; 
    time = timestamp() - time;  
    printf("Pi: %f\n", sum);
    printf("Tempo: %f\n", abs(time));
}