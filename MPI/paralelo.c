#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MASTER 0

void read_serie(char * arquivo, double *vetor, int tamanho){
    FILE *fp = fopen(arquivo, "r");
    if(fp==NULL){
        fprintf(stderr, "não foi possivel abrir o arquivo %s\n", arquivo);
        exit(1);
    }
    for(int i=0; i<tamanho; ++i){
        fscanf(fp, "%lf", &vetor[i]);
    }
}

void max_min_avg(double *vetor, int tamanho, double *max, double *min, double *media){
    double soma = 0;
    *max = *min = vetor[0];
    for(int i=0; i< tamanho; i++){
        soma += vetor[i];
        if (vetor[i] > *max)
            *max = vetor[i];
        if (vetor[i] < *min)
            *min = vetor[i];
    }
    *media = soma / tamanho;
}

int main(int argc, char **argv){

    clock_t start;
    start = clock();

    if (argc != 4){
        fprintf(stderr, "necessário 2 argumentos: %s <arquivo time series> <tamanho time series> <tamanho janela>\n", argv[0]);
        return 1;
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
     
    //local
    double max, min, media;
    
    //global
    double global_max, global_min, global_media;

    int tam_serie = atoi(argv[2]);
    int tam_janela = atoi(argv[3]);
    double *serie = (double *) malloc(sizeof(double)*tam_serie);

    // Leia a série do arquivo de entrada
    if(rank == MASTER)
        read_serie(argv[1], serie, tam_serie);
    
    // Distribua a série para todos os processos
    MPI_Bcast(serie, tam_serie, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    if(rank == MASTER)
        printf("tamanho da serie: %d, tamanho da janela: %d\n",tam_serie, tam_janela);
   
    // Calcule o valor máximo, mínimo e média para a série inteira
    max_min_avg(serie,tam_serie, &max, &min, &media);
    
    if(rank == MASTER)
        printf("serie total - max: %lf, min: %lf, media: %lf\n", max, min, media);

    // Calcule o valor máximo, mínimo e média para cada janela
    for(int i = rank; i <= tam_serie - tam_janela; i += tam_janela){
        max_min_avg(&serie[i],tam_janela, &max, &min, &media);
        printf("janela %d - max: %lf, min: %lf, media: %lf\n", i, max, min, media);
    }

    if(rank == MASTER){
        printf("seconds: %lf\n", (double)(clock() - start)/CLOCKS_PER_SEC);
    }

    MPI_Finalize();
}