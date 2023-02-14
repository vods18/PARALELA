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

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, w_rank%2, w_rank, &new_comm);

    int num_proc;
    MPI_Comm_size(new_comm, &num_proc);

    int rank;
    MPI_Comm_rank(new_comm, &rank);

    printf("World rank: %d, new_comm rank: %d, new_comm size: %d\n",w_rank,rank,num_proc);

    MPI_Finalize();
}
