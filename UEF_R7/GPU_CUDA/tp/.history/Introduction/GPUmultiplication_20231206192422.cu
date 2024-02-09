#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda_runtime.h>

// Kernel function to add the elements of two matrices
__global__ void add(int rows, int cols, float *matA, float *matB, float *matC)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows && col < cols)
    {
        int index = row * cols + col;
        matC[index] = matA[index] + matB[index];
    }
}

// kernel function to multiply two matrices
__global__ void multiply(int rows, int cols, float *matA, float *matB, float *matC)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows && col < cols)
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
    // time start
    clock_t start = clock();
    // Copy the arrays to the device[GPU]
    cudaMemcpy(d_matA, matA, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, N * sizeof(float), cudaMemcpyHostToDevice);

    // Perform addition on the GPU
    dim3 blockSize(16, 16);                       // number of threads per block
    dim3 numBlocks(ceil((float)(cols)) / blockSize.x,  
                   ceil((float)(rows)) / blockSize.y);  // number of blocks
    multiply<<<numBlocks, blockSize>>>(rows, cols, d_matA, d_matB, d_matC); // call multiply kernel function

    // Copy the result back to the host
    cudaMemcpy(matC, d_matC, N * sizeof(float), cudaMemcpyDeviceToHost);
    clock_t end = clock(); // time end
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(matC[i] - 2048.0f));
    // print matA and matB and matC
    for (int i = 0; i < 10; i++)
    {
        printf("[GPU] matA[%d] = %f\n", i, matA[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("[GPU] matB[%d] = %f\n", i, matB[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("[GPU] matC[%d] = %f\n", i, matC[i]);
    }
    printf("[GPU] Max error: %f\n", maxError);
    printf("[GPU] Time spent: %f\n", time_spent);
    // Free memory
    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_matC);
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
