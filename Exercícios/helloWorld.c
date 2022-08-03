#include <stdio.h>
#include <omp.h>

int main (){

    #pragma omp parallel
    {
        int ID = omp_get_thread_num(); // variavel local de cada thread, uma forma de definir uma variavel privada
        printf(" hello(%d) ", ID);
        printf(" world(%d) \n", ID);
    }
}