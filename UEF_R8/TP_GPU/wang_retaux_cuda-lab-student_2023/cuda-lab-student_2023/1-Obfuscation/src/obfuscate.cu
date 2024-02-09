#include <stdint.h>
#include <cuda.h>
#include <iostream>
#include "obfuscate.hpp"

/************************************************************************************
                                        EXERCISE 1
*************************************************************************************/

/**
 * @brief This function should add the values of the pixels from two input buffers and store the result in an output buffer
 *        This function should be executed on the GPU, using CUDA
 * 
 * @param d_in_buffer_1 : input image 1, contains the values of the pixels
 * @param d_in_buffer_2 : input image 2, contains the values of the pixels  
 * @param width         : width of the image
 * @param height        : height of the image
 * @param d_out_buffer  : output image, contains the values of the pixels
 * @return __global__ 
 */
__global__ void k_cuda_exercise_1(uint8_t *d_in_buffer_1, uint8_t *d_in_buffer_2, uint32_t width, uint32_t height, uint8_t *d_out_buffer)
{
    // add an element coresponding to the thread index (hint: use threadIdx.x and threadIdx.y)

    // index_x and index_y are the indices of the element in the buffer
    int index_x = threadIdx.x + blockDim.x*blockIdx.x;
    int index_y = threadIdx.y + blockDim.y*blockIdx.y;
    if(index_x < width && index_y < height)
    {
        // this is the index of the element in the buffer
        int index = index_x + index_y * width;
        d_out_buffer[index] = d_in_buffer_1[index] + d_in_buffer_2[index];
    }
}

/**
 * @brief This function should add the values of the pixels from two input buffers and store the result in an output buffer
 * 
 * @param in_buffer_1  : input image 1, contains the values of the pixels,not RGB
 * @param in_buffer_2  : input image 2, contains the values of the pixels,not RGB
 * @param width        : width of the image
 * @param height       : height of the image
 * @param out_buffer   : output image, contains the values of the pixels
 */
void cuda_exercise_1(uint8_t *in_buffer_1, uint8_t *in_buffer_2, uint32_t width, uint32_t height, uint8_t *out_buffer)
{
    uint8_t *d_in_buffer_1 = NULL, *d_in_buffer_2 = NULL, *d_out_buffer = NULL;
    const int size_bytes = width * height * sizeof(uint8_t);
    std::cout << "width: " << width << " height: " << height << std::endl;


    cudaEventRecord(start_memory);
    //allocate memory for d_in_buffer_1, d_in_buffer_2 and d_out_buffer
    cudaMalloc(&d_in_buffer_1, size_bytes);
    cudaMalloc(&d_in_buffer_2, size_bytes);
    cudaMalloc(&d_out_buffer, size_bytes);
    //copy in_buffer_1 and in_buffer_2 to device
    cudaMemcpy(d_in_buffer_1, in_buffer_1, size_bytes, cudaMemcpyHostToDevice);    
    cudaMemcpy(d_in_buffer_2, in_buffer_2, size_bytes, cudaMemcpyHostToDevice);

    cudaEventRecord(start_kernel);
    //launch kernels with the right block dimensions
    dim3 dimBlock(12,16);
    dim3 dimGrid(ceil(width/12.0), ceil(height/16.0));
    std::cout << "dimBlock: " << dimBlock.x << " " << dimBlock.y << std::endl;
    std::cout << "dimGrid: " << dimGrid.x << " " << dimGrid.y << std::endl;
    k_cuda_exercise_1<<<dimGrid, dimBlock>>>(d_in_buffer_1, d_in_buffer_2, width, height, d_out_buffer);

    cudaEventRecord(start_copyback);
    //copy d_out_buffer back to host
    cudaMemcpy(out_buffer, d_out_buffer, size_bytes, cudaMemcpyDeviceToHost);

    //free all the device buffers
    cudaFree(d_out_buffer);
    cudaFree(d_in_buffer_1);
    cudaFree(d_in_buffer_2);

    cudaEventSynchronize(end);
}

/************************************************************************************
                                        EXERCISE 2
*************************************************************************************/

/**
 * @brief This function should add the R, G and B values of the pixels from two input buffers and store the result in an output buffer
 *        This function should be executed on the GPU, using CUDA
 *        So each thread should add the R, G and B values of a pixel from the two input buffers and store the result in the output buffer
 * 
 * @param d_in_buffer_1  : input image 1, contains the R, G and B values of the pixels
 * @param d_in_buffer_2  : input image 2, contains the R, G and B values of the pixels
 * @param width          : width of the image
 * @param height         : height of the image
 * @param d_out_buffer   : output image, contains the R, G and B values of the pixels
 * @return __global__ 
 */
__global__
void k_cuda_exercise_2(uint32_t *d_in_buffer_1, uint32_t *d_in_buffer_2, uint32_t width, uint32_t height, uint32_t *d_out_buffer)
{
    //add an element coresponding to the thread index (hint: use blockIdx.x, blockIdx.y, threadIdx.x and threadIdx.y)
    int index_x = threadIdx.x + blockDim.x * blockIdx.x;
    int index_y = threadIdx.y + blockDim.y * blockIdx.y;
    if(index_x <= width && index_y <= height)
    {
        // this is the index of the element in the buffer
        int index = index_x + index_y * width;
        // get the R, G and B values of the pixel from the two input buffers
        uint8_t R =  get_R(d_in_buffer_1[index]) + get_R(d_in_buffer_2[index]);
        uint8_t G =  get_G(d_in_buffer_1[index]) + get_G(d_in_buffer_2[index]);
        uint8_t B =  get_B(d_in_buffer_1[index]) + get_B(d_in_buffer_2[index]);
        // set the R, G and B values of the pixel in the output buffer
        d_out_buffer[index] = make_RGB(R, G, B);
    }
}


/**
 * @brief This function should add the R, G and B values of the pixels from two input buffers and store the result in an output buffer
 * 
 * @param in_buffer_1 : input image 1, contains the R, G and B values of the pixels
 * @param in_buffer_2 : input image 2, contains the R, G and B values of the pixels
 * @param width      : width of the image
 * @param height    : height of the image
 * @param out_buffer : output image, contains the R, G and B values of the pixels
 */
void cuda_exercise_2(uint32_t *in_buffer_1, uint32_t *in_buffer_2, uint32_t width, uint32_t height, uint32_t *out_buffer)
{
    uint32_t *d_in_buffer_1 = NULL, *d_in_buffer_2 = NULL, *d_out_buffer = NULL;
    const int size_bytes = width * height * sizeof(uint32_t);

    std::cout << "width: " << width << " height: " << height << std::endl;

    cudaEventRecord(start_memory);
    // allocate memory for d_in_buffer_2 and d_out_buffer
    cudaMalloc(&d_in_buffer_1, size_bytes);
    cudaMalloc(&d_in_buffer_2, size_bytes);
    cudaMalloc(&d_out_buffer, size_bytes);

    // copy in_buffer_2 to device
    cudaMemcpy(d_in_buffer_1, in_buffer_1, size_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_in_buffer_2, in_buffer_2, size_bytes, cudaMemcpyHostToDevice);

    cudaEventRecord(start_kernel);
    //launch kernels with the right block and grid dimensions
    dim3 dimBlock(10, 10);
    dim3 dimGrid(ceil(width/10.0), ceil(height/10.0));
    std::cout << "dimBlock: " << dimBlock.x << " " << dimBlock.y << std::endl;
    std::cout << "dimGrid: " << dimGrid.x << " " << dimGrid.y << std::endl;
    k_cuda_exercise_2<<<dimGrid, dimBlock>>>(d_in_buffer_1, d_in_buffer_2, width, height, d_out_buffer);

    cudaEventRecord(start_copyback);

    cudaEventRecord(start_copyback);
    // copy d_out_buffer back to host
    cudaMemcpy(out_buffer, d_out_buffer, size_bytes, cudaMemcpyDeviceToHost);

    cudaEventRecord(end);
    // free all the device buffers
    cudaFree(d_in_buffer_1);
    cudaFree(d_in_buffer_2);
    cudaFree(d_out_buffer);

    cudaEventSynchronize(end);
}