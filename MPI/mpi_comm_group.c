#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int w_num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &w_num_proc);

    int w_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &w_rank);

    printf("World rank %d of %d processes in MPI_COMM_WORLD\n",
           w_rank, w_num_proc);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int n = 2;
    const int ranks[2] = {0, 1};
    MPI_Group novo_grupo; // grupo com processos 0 e 1
    MPI_Group_incl(world_group, n, ranks, &novo_grupo);

    MPI_Group grupo_diff; // grupo com os demais processos
    MPI_Group_difference(world_group, novo_grupo, &grupo_diff);

    MPI_Comm novo_comm;
    int num_proc;
    int rank;
    if(w_rank == 0 || w_rank == 1){
        MPI_Comm_create_group(MPI_COMM_WORLD, novo_grupo, 0, &novo_comm);
        MPI_Comm_size(novo_comm, &num_proc);
        MPI_Comm_rank(novo_comm, &rank);
    } else {
        MPI_Comm_create_group(MPI_COMM_WORLD, grupo_diff, 0, &novo_comm);
        MPI_Comm_size(novo_comm, &num_proc);
        MPI_Comm_rank(novo_comm, &rank);
    }
    printf("World rank: %d, novo_comm rank: %d, novo_comm size: %d\n",w_rank,rank,num_proc);

    MPI_Finalize();
}
