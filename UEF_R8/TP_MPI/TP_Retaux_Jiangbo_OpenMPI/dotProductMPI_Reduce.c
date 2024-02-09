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
    int *v1_local = malloc(local_n * sizeof(int));
    int *v2_local = malloc(local_n * sizeof(int));
    // Initialize vectors with some values in each process
    for (int i = 0; i < local_n; ++i)
    {
        v1_local[i] = 3 * 1.0;
        v2_local[i] = 3.0;
    }

    // Each process will perform the dot product on a part of the vectors
    local_sum = dotproduct(v1_local, v2_local, local_n);
    printf("local_sum = %d\n", local_sum);
    int sum_global = 0;
    // MPI_Reduce is used to sum(MPI_SUM) all the local sums from each process and store the result in sum_global
    MPI_Reduce(&local_sum, &sum_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Global sum for dot product with MPI_Reduce = %d\n", sum_global); // print the global sum for the master process
    }
    // free the memory for each process
    free(v1_local);
    free(v2_local);
    MPI_Finalize();
    return 0;
}
