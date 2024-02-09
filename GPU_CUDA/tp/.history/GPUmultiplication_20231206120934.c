#include <iostream>
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

    cudaMemcpy(d_matA, matA, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, N * sizeof(float), cudaMemcpyHostToDevice);

    // Perform addition on the GPU
    dim3 blockSize(16, 16);
    dim3 numBlocks((cols + blockSize.x - 1) / blockSize.x,
                   (rows + blockSize.y - 1) / blockSize.y);
    multiply<<<numBlocks, blockSize>>>(rows, cols, d_matA, d_matB, d_matC);

    // Copy the result back to the host
    cudaMemcpy(matC, d_matC, N * sizeof(float), cudaMemcpyDeviceToHost);

    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(matC[i] - 2048.0f));
    std::cout << "Max error: " << maxError << std::endl;

    // Free memory
    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_matC);
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
