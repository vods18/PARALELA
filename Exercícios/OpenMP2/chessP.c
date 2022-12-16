#include <stdio.h>
#include <time.h>
#include <omp.h>

#define N 5
#define M 5

void print_tabuleiro(int tabuleiro[N][M]){
    int i, j;
    for (i=0; i < N; i++){
        for (j=0; j < M; j++)
            printf("%3d ",tabuleiro[i][j]);
        printf("\n");
    }
}

int jogada_valida(int x, int y, int tabuleiro[N][M]){
    if (x < 0  || x >= N || y < 0 || y >= M)
        return 0;
    if(tabuleiro[x][y] != 0)
        return 0;
    return 1;
}

/* Possiveis movimentos
#1
x2 = x + 1;
y2 = y + 2;

#2
x2 = x + 1;
y2 = y - 2;

#3
x2 = x - 1;
y2 = y + 2;

#4
x2 = x - 1;
y2 = y - 2;

#5
x2 = x + 2;
y2 = y + 1;

#6
x2 = x + 2;
y2 = y - 1;

#7
x2 = x - 2;
y2 = y + 1;

#8
x2 = x - 2;
y2 = y - 1;
*/
int proximo_movimento_y(int y, int movimento){
    int valor = 1;
    if( movimento < 5 )
        valor = 2;
    if (movimento % 2 == 0) // se par, eh uma subtracao
        return y - valor;
    else
        return y + valor;
    
}
int proximo_movimento_x(int x, int movimento){
    if (movimento < 3)
        return x + 1;
    else if (movimento < 5)
        return x - 1;
    else if (movimento < 7)
        return x + 2;
    else 
        return x - 2;
}
int passeio_cavalo(int tabuleiro[N][M], int x, int y, int jogada){
    int x2, y2, i, result = 0, result1 = 0, result2 = 0, result3 = 0;
    if (jogada == N*M)
        return 1;
    else if (jogada == (N*M)/0.4){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tabuleiro)){
            tabuleiro[x2][y2] = jogada+1;
            if(passeio_cavalo(tabuleiro, x2,y2, jogada+1))
                return 1;
            tabuleiro[x2][y2] = 0;
    }

    }
    else{
        for (i=1;i<9;i++){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
            if (jogada_valida(x2,y2, tabuleiro)){
                tabuleiro[x2][y2] = jogada+1;
                        
                    #pragma omp task shared(x2,y2)
                    omp_get_thread_num();
                    //printf("%i ", omp_get_thread_num());
                    //printf("\n");
                    result = passeio_cavalo(tabuleiro, x2,y2, jogada+1);
                        //printf("%i\n", jogada+1);
                
                #pragma omp taskwait
                if (!result){
                    tabuleiro[x2][y2] = 0;
                }
            }
        }
    }
    return result; 
}

int main(){
    int i, j;
    int tabuleiro[N][M];
    int x_inicio = 0, y_inicio = 0;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    printf("Resolvendo para N=%d e M=%d\n",N,M);

    for (i=0; i < N; i++)
        for (j=0; j < M; j++)
            tabuleiro[i][j] = 0;

    tabuleiro[x_inicio][y_inicio] = 1;
    #pragma omp parallel
    {
        #pragma omp single
        {
            if(passeio_cavalo(tabuleiro, x_inicio, y_inicio, 1))
                print_tabuleiro(tabuleiro);
            else
                printf("Nao existe solucao\n");
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f seconds\n",cpu_time_used);
}
