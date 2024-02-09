
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
// multiply two matrices on the CPU
// matC[index] += matA[row * cols + k] * matB[k * cols + col];
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
    clock_t start = clock();
    CPUMultiply(rows, cols, matA, matB, matC);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(matC[i] - 2048.0f));

    // print matA and matB and matC
    for (int i = 0; i < N; i++)
    {
        printf("matA[%d] = %f", i, matA[i]);
    }
    for (int i = 0; i < N; i++)
    {
        printf("matB[%d] = %f\n", i, matB[i]);
    }
    for (int i = 0; i < N; i++)
    {
        printf("matC[%d] = %f\n", i, matC[i]);
    }
    printf("[CPU] Max error: %f\n", maxError);
    printf("Time spent: %f\n", time_spent);

    // Free memory
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
