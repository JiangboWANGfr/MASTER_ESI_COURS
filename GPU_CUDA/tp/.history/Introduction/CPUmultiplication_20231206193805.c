/**
 * @file CPUmultiplication.c
 * @author jiangbo WANG (jiangbo.wang@ensea.fr)
 * @brief this file is to perform matrix multiplication on the CPU
 *        and compare the results with the GPU results(see GPUmultiplication.c)
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/**
 * @brief this function is to perform matrix multiplication on the CPU
 * @note  we using 3 for loops to perform matrix multiplication
 *        the first for loop is to go through the rows of matrix A
 *        the second for loop is to go through the columns of matrix B
 *        the third for loop is to go through the columns of matrix A and rows of matrix B to perform the multiplication
 *        the index is the index of the element of matrix C ,which is calculated by the row and column
 * @param rows number of rows
 * @param cols number of columns
 * @param matA matrix A
 * @param matB matrix B
 * @param matC matrix C
 */
void CPUMultiply(int rows, int cols, float *matA, float *matB, float *matC)
{
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++)
        {
            int index = row * cols + col;
            for (int k = 0; k < cols; k++)
            {
                matC[index] += matA[row * cols + k] * matB[k * cols + col];
            }
        }
}

/**
 * @brief main function
 *      0. allocate memory on the host for matA, matB, and matC
 *      1. initialize matA and matB arrays on the host
 *      2. perform multiplication on the CPU
 *      3. check for errors (all values Of matC should be 2048.0f)
 *      4. print matA and matB and matC
 *     5. free memory
 *@note the time spent on the CPU is about 98.6s ,this is only the time of multiplication
 * @return int
 */
int main(void)
{
    int rows = 1024; // number of rows
    int cols = 1024; // number of columns
    int N = rows * cols;

    float *matA, *matB, *matC;
    matA = (float *)malloc(N * sizeof(float));
    matB = (float *)malloc(N * sizeof(float));
    matC = (float *)malloc(N * sizeof(float));

    // Initialize matA and matB arrays on the host
    for (int i = 0; i < N; i++)
    {
        matA[i] = 1.0f;
        matB[i] = 2.0f;
    }

    // Perform multiplication on the CPU
    clock_t start = clock();                                    // start time
    CPUMultiply(rows, cols, matA, matB, matC);                  // CPU multiplication
    clock_t end = clock();                                      // end time
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC; // time spent

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(matC[i] - 2048.0f));

    // print matA and matB and matC
    for (int i = 0; i < 10; i++)
    {
        printf("[CPU] matA[%d] = %f\n", i, matA[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("[CPU] matB[%d] = %f\n", i, matB[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("[CPU] matC[%d] = %f\n", i, matC[i]);
    }
    printf("[CPU] Max error: %f\n", maxError);
    printf("[CPU] Time spent: %f s\n", time_spent);

    // Free memory
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
