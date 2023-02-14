#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

#define TAM 2

// Funcao para imprimir toda a mensagem em apenas um unico printf
// Garantindo que a mensagem nao serah cortada por uma mensagem
// de outro processo
void imprime_vetor(int rank, int *vetor, int tamanho, char *msg_inicial) {
    int i;
    char *str, strT[20];
    str = (char*)malloc(sizeof(char)*tamanho*20);
    str[0]='\0';
    // Salva o vetor em uma unica string
    for(i=0; i<tamanho; ++i) {
        sprintf(strT, "%d ",vetor[i]);
        strcat(str,strT);
    }

    printf("%s processo %d, vetor: %s\n",msg_inicial,rank,str);
    free(str);
}

int main(int argc, char** argv) {
    int i;
    int *vetorTotal, *vetorParte;

    MPI_Init(&argc, &argv);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL)*rank);
    // Inicializa o vetor apenas no processo 0

    vetorParte = (int*) malloc(sizeof(int)*TAM);
    for(i=0; i<TAM; ++i)
        vetorParte[i] = rand()%50;
    imprime_vetor(rank, vetorParte, TAM, "Vetor parte antes do gather,");

    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0) {
        vetorTotal = (int*) malloc(sizeof(int)*TAM*num_proc);
        imprime_vetor(rank,vetorTotal,TAM*num_proc, "Vetor total antes do gather,");
    }

    // Copia o vetor do processo 0 para todos os processos no comunicador MPI_COMM_WORLD
    MPI_Gather(vetorParte, TAM, MPI_INT, vetorTotal, TAM, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        imprime_vetor(rank,vetorTotal,TAM*num_proc, "Vetor total apos gather,");
    }

    free(vetorParte);
    if(rank == 0)
        free(vetorTotal);
    MPI_Finalize();
}
