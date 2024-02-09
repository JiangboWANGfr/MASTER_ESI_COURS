/**
 * @file GPUmultiplication.cu
 * @author jiangboWANG(jiangbo.wang@ensea.fr)
 * @brief  GPU multiplication of two matrices of size 1024*1024 using CUDA
 *          we campare the time spent between CPU and GPU(see CPUmultiplication.c)
 * @version 0.1
 * @date 2023-12-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda_runtime.h>


/**
 * @brief kernel function to multiply two matrices
 * @details in the GPU, we use the grid of blocks and threads to compute the 
 *          multiplication of two matrices ,so we just need to use one for loop.
 *          1. we calculate the row and column indices of each thread, this is done using 
 *             the block index (blockIdx.x and blockIdx.y), block dimension (blockDim.x 
 *             and blockDim.y), and thread index within the block (threadIdx.x and threadIdx.y). 
 * 
 * 
 * @param rows number of rows
 * @param cols number of columns
 * @param matA matrix A
 * @param matB matrix B
 * @param matC matrix C = matA * matB
*/
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

/**
 * @brief main function
 *        0. allocate memory on the host and device
 *        1. initialize matA and matB on the host 
 *        2. copy matA and matB from the host to the device[GPU]
 *        3. define the number of threads per block and the number of blocks
 *        4. call the kernel function
 *        5. copy the result from the device to the host
 *        6. print the result
 *        7. free memory on the host and device
 * @note the time spent is 0.025s using GPU ,compare to 98.56s using CPU
 * @return int 
*/
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
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC; // calculate time spent
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
