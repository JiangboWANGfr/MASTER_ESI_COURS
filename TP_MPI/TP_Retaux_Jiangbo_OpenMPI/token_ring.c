#include <mpi.h> //required to use MPl functions
#include <stdio.h>

/**
 * @brief This is the token ring program
 * @result the token is sent from process 0 to process 1, then from process 1 to process 2,
 *         then from process 2 to process 3, then from process 3 to process 0,the result is: as follows:
            I am process #0/4, I send the token to process 1
            I am process #1/4, I receive the token from process 0
            I am process #1/4, I send the token to process 2
            I am process #2/4, I receive the token from process 1
            I am process #2/4, I send the token to process 3
            I am process #3/4, I receive the token from process 2
            I am process #3/4, I send the token to process 0
            I am process #0/4, I receive the token from process 3
 * @param argc
 * @param argv
 */
void main(int argc, char *argv[])
{
    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int MaxRing = size - 1;
    int token = 0;
    if (rank == 0)
    {
        token = 1;
        MPI_Send(&token, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); // send the token to the process 1
        printf("I am process #%d/%d, I send the token to process %d\n", rank, size, rank + 1);
        MPI_Recv(&token, 1, MPI_INT, MaxRing, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the token from the process MaxRing
        printf("I am process #%d/%d, I receive the token from process %d\n", rank, size, MaxRing);
    }
    else
    {
        if (rank < MaxRing)
        {
            MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the token from the process rank - 1
            printf("I am process #%d/%d, I receive the token from process %d\n", rank, size, rank - 1);
            MPI_Send(&token, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); // send the token to the process rank + 1
            printf("I am process #%d/%d, I send the token to process %d\n", rank, size, rank + 1);
        }
        else
        {
            MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the token from the process rank - 1
            printf("I am process #%d/%d, I receive the token from process %d\n", rank, size, rank - 1);
            MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send the token to the process 0
            printf("I am process #%d/%d, I send the token to process %d\n", rank, size, 0);
        }
    }
    // must ALWAYS be called to run an MPI program
    MPI_Finalize();
    return 0;
}