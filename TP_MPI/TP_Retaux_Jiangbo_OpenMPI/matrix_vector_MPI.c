#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @brief This function performs the matrix-vector product in sequential for each process
 * @note This function is called by each process, all the processes being sent in the matvec_mpi() function
 *       each process will perform the matrix-vector product on a part of the matrix, in our case ,each process
 *       will perform the product on rows/size(10000/4=2500) rows
 *
 * @param matrix : matrix of size rowsPerProcess x cols
 * @param vector : vector of size cols
 * @param result : vector of size rowsPerProcess
 * @param rows   :  number of rows of the matrix
 * @param cols   : number of columns of the matrix
 */
void matvec(int **matrix, int *vector, int *result, int rowsPerProcess, int cols, int rank)
{
    // start timer
    clock_t start = clock();
    for (int i = rowsPerProcess * rank; i < rowsPerProcess * (rank + 1); i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("rank %d: time spent = %f ", rank, time_spent);
}

/**
 * @brief This is the parallel version of the matrix-vector product by using OpenMPI
 * @note  The matrix and the vector is initialized in the master process, then the master process sends the sub-matrices
 *        and the vector to each process,so that each process can perform the matrix-vector product on a part of the matrix
 *@note   In this version, the master process sends the vector to each process by using MPI_Send() function,
          and each process receives the vector by using MPI_Recv() function,
          after calculating the matrix-vector product, each process sends the result to the master process
          by using MPI_Send() function, and the master process receives the result by using MPI_Recv() function
 *
 * @param matrix : matrix of size rows x cols
 * @param vector : vector of size cols
 * @param result : vector of size rows
 * @param rows   : number of rows of the matrix
 * @param cols   : number of columns of the matrix
 * @param rank   : rank of the process
 * @param size   : number of processes
 */
void matvec_mpi(int **matrix, int *vector, int *result, int rows, int cols, int rank, int size)
{

    int rowsPerProcess = rows / size; // 10000/4 = 2500
    if (rank == 0)
    {
        printf("rowsPerProcess = %d\n", rowsPerProcess);

        for (int i = 1; i < size; i++)
        {
            MPI_Send(matrix[rowsPerProcess * i], (rowsPerProcess + 1) * cols, MPI_INT, i, 0, MPI_COMM_WORLD); // send the sub-matrices to each process
            MPI_Send(vector, cols, MPI_INT, i, 0, MPI_COMM_WORLD);                                            // send the vector to each process
        }
        matvec(matrix, vector, result, rowsPerProcess, cols, rank);
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(result + rowsPerProcess * i, rowsPerProcess, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the result from each process
        }
    }
    else
    {
        MPI_Recv(matrix[rowsPerProcess * rank], (rowsPerProcess + 1) * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the sub-matrix from the master process
        MPI_Recv(vector, cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                                               // receive the vector from the master process
        matvec(matrix, vector, result, rowsPerProcess, cols, rank);                                                             // perform the matrix-vector product
        MPI_Send(result + rowsPerProcess * rank, rowsPerProcess, MPI_INT, 0, 0, MPI_COMM_WORLD);                                // send the result to the master process
    }
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
    int rows = 120; // Example size
    int cols = 100;
    // Allocate and initialize matrix and vectors
    int **matrix = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    int *vector = malloc(cols * sizeof(int));
    int *result = malloc(rows * sizeof(int));

    if (rank == 0)
    {
        // Initialize matrix with some values
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] = 2;
            }
        }

        // Initialize vector with some values
        for (int i = 0; i < cols; i++)
        {
            vector[i] = 3;
        }
        for (int i = 0; i < rows; i++)
        {
            result[i] = 0;
        }
    }
    else
    {
        // Initialize matrix with some values
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] = 0;
            }
        }

        // Initialize vector with some values
        for (int i = 0; i < cols; i++)
        {
            vector[i] = 0;
        }
        for (int i = 0; i < rows; i++)
        {
            result[i] = 0;
        }
    }

    matvec_mpi(matrix, vector, result, rows, cols, rank, size);

    // wait for all processes to finish
    int sumResult = 0.0;
    for (int i = 0; i < rows; i++)
    {
        sumResult += result[i];
    }
    printf("rank %d: sumResult = %d\n", rank, sumResult);
    if (rank == 0)
    {
        int sumResultGlobal = 0.0;
        for (int i = 0; i < rows; i++)
        {
            sumResultGlobal += result[i];
        }
        printf("sum Result Global with MPI = %d\n", sumResultGlobal);
    }
    free(matrix);
    free(vector);
    free(result);
    MPI_Finalize();

    return 0;
}
