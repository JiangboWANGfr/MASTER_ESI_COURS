#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @brief This function performs the matrix-vector product in sequential
 *
 * @param matrix : matrix of size rows x cols
 * @param vector : vector of size cols
 * @param result : vector of size rows
 * @param rows   : number of rows of the matrix
 * @param cols   : number of columns of the matrix
 */
void matvec(double **matrix, double *vector, double *result, int rows, int cols)
{
    // start timer
    clock_t start = clock();
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent without MPI version : %f\n", time_spent);
}

int main()
{
    int rows = 100; // Example size
    int cols = 100;

    // Allocate and initialize matrix and vectors
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    // Initialize matrix with some values
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = 2.0;
        }
    }

    double *vector = (double *)malloc(cols * sizeof(double));
    // Initialize vector with some values
    for (int i = 0; i < cols; i++)
    {
        vector[i] = 3.0;
    }

    double *result = (double *)malloc(rows * sizeof(double));

    matvec(matrix, vector, result, rows, cols);
    double sumResult = 0.0;
    for (int i = 0; i < rows; i++)
    {
        sumResult += result[i]; // Sum the result to compare with the parallel version
    }
    printf("Sum result: %f\n", sumResult); // Print the sum of the result

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
