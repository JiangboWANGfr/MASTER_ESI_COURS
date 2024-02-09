#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

/**
 * @brief This function performs the dot product of two vectors in sequential
 * @note  This sequential version is used to compare with the parallel version
 *
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n  : size of the vectors
 * @return double
 */
double dotproduct(double *v1, double *v2, int n)
{
    // start timer
    clock_t start = clock();
    double sum = 0.0;

    for (int i = 0; i < n; ++i)
        sum += v1[i] * v2[i];
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Normal Time spent normal: %f\n", time_spent);
    return sum;
}

int main(void)
{
    int n = 100000000;
    double *v1 = malloc(n * sizeof(double));
    double *v2 = malloc(n * sizeof(double));
    // Initialize vectors with some values, here we use 3.0 and 2.0
    for (int i = 0; i < n; ++i)
    {
        v1[i] = 3 * 1.0;
        v2[i] = 2.0;
    }
    double sum = dotproduct(v1, v2, n); // perform the dot product on the vectors v1 and v2
    printf("sum = %f\n", sum);
    free(v1);
    free(v2);
    return 0;
}
