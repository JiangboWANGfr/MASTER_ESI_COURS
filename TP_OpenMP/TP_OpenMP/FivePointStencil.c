#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @brief This function performs the matrix-vector product in sequential
 *        The seauential version is executed in 0.222 seconds
 *
 * @param matrix : matrix of size rows x cols
 * @param vector : vector of size cols
 * @param result : vector of size rows
 * @param rows : number of rows of the matrix
 * @param cols : number of columns of the matrix
 */
void matvec(double **matrix, double *vector, double *result, int rows, int cols)
{
    // start timer
    clock_t start = clock(); // let's start the timer here
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    clock_t end = clock(); // let's stop the timer here
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent for sequential version : %f\n", time_spent); //print the running time
}

/**
 * @brief This function performs the matrix-vector product in parallel with openmp
 *      The parallel version is executed in 0.090 seconds
 * @note  For the parallel version, we split the rows of the matrix into a certain number of threads by using the (omp for) pragma
 *        The threads will be synchronized at the end of the for loop
 * @note This enables speeding up the execution time by a factor of 2.5
 * @param matrix : matrix of size rows x cols
 * @param vector : vector of size cols
 * @param result : vector of size rows
 * @param rows : number of rows of the matrix
 * @param cols : number of columns of the matrix
 */
void matvec_omp(double **matrix, double *vector, double *result, int rows, int cols)
{
    // start timer
    double start = omp_get_wtime(); // let's start the timer here
#pragma omp parallel default(none) shared(matrix, vector, result, rows, cols) // these variables are shared between the threads
    {
#pragma omp for // here begins the data distribution across the threads
        for (int i = 0; i < rows; i++)
        {
            result[i] = 0.0;
            for (int j = 0; j < cols; j++)
            {
                result[i] += matrix[i][j] * vector[j];
            }
        }
    }
    double end = omp_get_wtime(); // let's stop the timer here
    double time_spent = (double)(end - start);
    printf("Time spent for parallel version : %f\n", time_spent);  //print the running time
}

int main()
{
    int rows = 10000; // Example numbers of rows and cols
    int cols = 10000;

    // Allocate and initialize matrix and vectors
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    // Initializing the matrix with some values
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = 2.0;
        }
    }

    double *vector = (double *)malloc(cols * sizeof(double));
    // Initializing the vector with some values
    for (int i = 0; i < cols; i++)
    {
        vector[i] = 3.0;
    }

    double *result = (double *)malloc(rows * sizeof(double));

#ifdef OMP // Perform the matrix-vector product by using parallelism
    matvec_omp(matrix, vector, result, rows, cols);
#else // Perform the matrix-vector product sequentially
    matvec(matrix, vector, result, rows, cols);
#endif
    double sumResult = 0.0;
    for (int i = 0; i < rows; i++) // Here we add all the elements of the result vector to compare the results of the two versions
    {
        sumResult += result[i];
    }
    printf("Sum result: %f\n", sumResult);  //print the sum of the result vector

    // Free memory
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
