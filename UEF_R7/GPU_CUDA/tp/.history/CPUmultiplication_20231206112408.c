#include <stdio.h>  
#include <stdlib.h>


//  mutiply two matrix on the CPU
void CPUmultiplication(int rows, int cols, float *matA, float *matB, float *matC) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++){
            float sum = 0.0f;
            for (int i = 0; i < cols; i++) {
                sum += matA[row * cols + i] * matB[i * cols + col];
            }
            matC[row * cols + col] = sum;
        }
    }
}

