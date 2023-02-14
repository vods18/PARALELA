/*
 * To define TAM and print the random vector:
 * $ mpicc -DTAM=50 -DDEBUG=1 max_mpi.c -o max_mpi
 *
 * To execute without printing the vector:
 * $ mpicc -DTAM=50 max_mpi.c -o max_mpi
 *
 * To run the executable
 *
 * $ mpirun -np 2 ./max_mpi
 *
*/
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#ifndef TAM
#define TAM 20
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc, char** argv) {
    int vetor[TAM], i;
    srand(time(0));

    MPI_Init(&argc, &argv);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(TAM % num_proc != 0) {
        fprintf(stderr, "ERROR, number of processes (%d) do not divide vector size (%d)\n",num_proc,TAM);
        return 1;
    }

    // Cria o vetor no rank 0, e envia parte dos dados para cada processo
    int chunck_size = TAM / num_proc;
    if (rank == 0) {
        for(i=0; i < TAM; ++i) {
            vetor[i] = rand() % TAM;
            if (DEBUG) printf("%d ",vetor[i]);
        }
        if (DEBUG) printf("\n");
        for(i=1; i<num_proc; ++i) {
            MPI_Send(&vetor[chunck_size*i], chunck_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(vetor, chunck_size, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    // Cada processo computa o max na sua parte dos dados
    int max = vetor[0];
    for(i=1; i<chunck_size; ++i) {
        if(vetor[i]>max)
            max = vetor[i];
    }

    int *max_results = (int *)malloc(num_proc*sizeof(int));
    // Cada processo envia o seu max para o processo 0
    if (rank == 0) {
        max_results[0] = max;
        for(i=1; i<num_proc; ++i) {
            MPI_Recv(&max_results[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }
    }
    else {
        MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Processo 0 verifica qual dos maiores eh de fato o maior
    if (rank == 0) {
        max = max_results[0];
        for(i=1; i<num_proc; ++i) {
            if(max_results[i]>max)
                max = max_results[i];
        }
        printf("max is %d\n",max);
    }

    MPI_Finalize();
}
