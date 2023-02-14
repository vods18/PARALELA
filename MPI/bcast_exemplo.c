#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

#define TAM 10

// Funcao para imprimir toda a mensagem em apenas um unico printf
// Garantindo que a mensagem nao serah cortada por uma mensagem
// de outro processo
void imprime_vetor(int rank, int *vetor, char *msg_inicial) {
    int i;
    char str[TAM*20]="", strT[20];
    // Salva o vetor em uma unica string
    for(i=0; i<TAM; ++i) {
        sprintf(strT, "%d ",vetor[i]);
        strcat(str,strT);
    }

    printf("%s processo %d, vetor: %s\n",msg_inicial,rank,str);
}

int main(int argc, char** argv) {
    int i, vetor[TAM];

    MPI_Init(&argc, &argv);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL));
    // Inicializa o vetor apenas no processo 0
    if(rank == 0)
        for(i=0; i<TAM; ++i)
            vetor[i] = rand()%50;

    imprime_vetor(rank, vetor, "Vetor antes do Bcast,");

    // Copia o vetor do processo 0 para todos os processos no comunicador MPI_COMM_WORLD
    MPI_Bcast(vetor, TAM, MPI_INT, 0, MPI_COMM_WORLD);

    imprime_vetor(rank, vetor, "Vetor apos Bcast,");

    MPI_Finalize();
}
