
#include <stdio.h>

// add two matrices on the CPU
void add(int rows, int cols, float *matA, float *matB, float *matC)
{
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++)
        {
            int index = row * cols + col;
            matC[index] = matA[index] + matB[index];
        }
}

// 

int main(void)
{
    int rows = 1024; // number of rows
    int cols = 1024; // number of columns
    int N = rows * cols;

    float *matA, *matB, *matC, *d_matA, *d_matB, *d_matC;
    matA = (float *)malloc(N * sizeof(float));
    matB = (float *)malloc(N * sizeof(float));
    matC = (float *)malloc(N * sizeof(float));

    cudaMalloc(&d_matA, N * sizeof(float));
    cudaMalloc(&d_matB, N * sizeof(float));
    cudaMalloc(&d_matC, N * sizeof(float));

    // Initialize matA and matB arrays on the host
    for (int i = 0; i < N; i++)
    {
        matA[i] = 1.0f;
        matB[i] = 2.0f;
    }

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(matC[i] - 3.0f));
    printf("Max error: %f\n", maxError);
    // Free memory
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
