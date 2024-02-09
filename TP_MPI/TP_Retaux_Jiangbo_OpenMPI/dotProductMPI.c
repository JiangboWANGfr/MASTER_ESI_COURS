#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

/**
 * @brief This function performs the dot product of two vectors
 * @note  This function is called by each process,and each process will perform the dot product on a part of the vectors,
 *        in our case ,each process will perform the dot product on n/size(6000000/4=1500000) elements
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n  : size of the vectors
 * @return int
 */
int dotproduct(int *v1, int *v2, int n)
{
    // start timer
    clock_t start = clock();
    int sum = 0.0;

    for (int i = 0; i < n; ++i)
        sum += v1[i] * v2[i];
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent: %f ", time_spent);
    return sum;
}

int main(int argc, char *argv[])
{
    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n = 6000000;        // Example size
    int local_n = n / size; // 6000000/4 = 1500000 for each process
    int local_sum = 0.0;    // local sum for each process to check the result of the dot product
    printf("rank %d: local_n = %d/%d ", rank, local_n, n);
    if (rank == 0)
    {
        int *v1 = malloc(n * sizeof(int));
        int *v2 = malloc(n * sizeof(int));
        int sum_global = 0;
        // Initialize vectors with some values in the master process
        for (int i = 0; i < n; ++i)
        {
            v1[i] = 3 * 1.0;
            v2[i] = 3.0;
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Send(v1 + local_n * i, local_n, MPI_INT, i, 0, MPI_COMM_WORLD); // send the sub-vectors v1 to each process
            MPI_Send(v2 + local_n * i, local_n, MPI_INT, i, 0, MPI_COMM_WORLD); // send the sub-vectors v2 to each process
        }
        local_sum = dotproduct(v1, v2, local_n); // perform the dot product on the sub-vectors v1 and v2 in the master process
        printf("local_sum = %d\n", local_sum);   // print the local sum for the master process
        // wait for all processes to finish
        sum_global += local_sum;
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&local_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the local sum from each process
            sum_global += local_sum;                                                   // sum the local sum from each process
        }
        printf("Global sum for dot product with MPI = %d\n", sum_global); // print the global sum for the dot product
        free(v1);
        free(v2);
    }
    else
    {
        int *v1_local = malloc(local_n * sizeof(int)); // allocate memory for the sub-vectors v1 and v2 in each process
        int *v2_local = malloc(local_n * sizeof(int));
        MPI_Recv(v1_local, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the sub-vectors v1 from the master process
        MPI_Recv(v2_local, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the sub-vectors v2 from the master process
        local_sum = dotproduct(v1_local, v2_local, local_n);                           // perform the dot product on the sub-vectors v1 and v2 in each process
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);                        // send the local sum to the master process
        free(v1_local);
        free(v2_local);
        printf("local_sum = %d\n", local_sum); // print the local sum for each process
    }

    // must ALWAYS be called to run an MPI program
    MPI_Finalize();
    return 0;
}
