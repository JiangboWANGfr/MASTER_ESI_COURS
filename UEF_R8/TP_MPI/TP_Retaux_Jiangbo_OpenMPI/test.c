#include <mpi.h> //required to use MPl functions
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("I am process #%d/%d\n", rank, size);
    // must ALWAYS be called to run an MPI program
    MPI_Finalize();
    return 0;
}