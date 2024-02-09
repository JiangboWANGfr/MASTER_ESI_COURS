#include <stdio.h>  
#include <stdlib.h>


//  add the elements of two matrices with CPU

void add(int rows, int cols, float *matA, float *matB, float *matC) {
    int index;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++){
            index = row * cols + col;
            matC[index] = matA[index] + matB[index];
        }
    }
}