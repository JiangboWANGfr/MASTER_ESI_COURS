#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

/**
 * @brief This function performs the dot product of two vectors in sequential
 * @note  The sequential version is executed in 0.172 seconds
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n : size of the vectors
 * @return double : the dot product of the two vectors
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

/**
 * @brief This function performs the dot product of two vectors in parallel with openmp
 * @note  The parallel version with critical is executed in 19.58 seconds
 * @note  The critical directive enables to ensure only one thread at a time modifies the shared variable
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n : size of the vectors
 * @return double : the dot product of the two vectors
 */
double dotproduct_omp_critical(double *v1, double *v2, int n)
{
    double sum = 0.0;
    double start = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
#pragma omp critical(sum)
        sum += v1[i] * v2[i];
    }
    double end = omp_get_wtime();
    double time_spent = end - start;
    printf("OpenMP Time spent critical : %f\n", time_spent);

    return sum;
}

/**
 * @brief This function performs the dot product of two vectors in parallel with openmp
 * @note  The parallel version with atomic is executed in 9.84 seconds
 * @note  The atomic directive enables to ensure only one thread at a time modifies the shared variable
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n : size of the vectors
 * @return double : the dot product of the two vectors
 */
double dotproduct_omp_atomic(double *v1, double *v2, int n)
{
    double sum = 0.0;
    double start = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < n; ++i)
    {
#pragma omp atomic
        sum += v1[i] * v2[i];
    }
    double end = omp_get_wtime();
    double time_spent = end - start;
    printf("OpenMP Time spent atomic : %f\n", time_spent);

    return sum;
}

/**
 * @brief This function performs the dot product of two vectors in parallel with openmp
 * @note The parallel version with reduction is executed in 0.112 seconds
 * @note  The reduction directive enables to ensure only one thread at a time modifies the shared variable
 * @note  For our example, the reduction version is the most efficient one ensuring the same result as the two previous ones
 * @param v1 : vector of size n
 * @param v2 : vector of size n
 * @param n : size of the vectors
 * @return double : the dot product of the two vectors
 */
double dotproduct_omp_reduction(double *v1, double *v2, int n)
{
    double sum = 0.0;
    double start = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += v1[i] * v2[i];
    }
    double end = omp_get_wtime();
    double time_spent = end - start;
    printf("OpenMP Time spent reduction : %f\n", time_spent);

    return sum;
}

int main(void)
{
    int n = 100000000; // Example size of the vectors
    double *v1 = malloc(n * sizeof(double));
    double *v2 = malloc(n * sizeof(double));
    // Initializing the vectors with some values,v1 = 3 and v2 = 2
    for (int i = 0; i < n; ++i)
    {
        v1[i] = 3 * 1.0;
        v2[i] = 2.0;
    }
#ifdef OMP                                            // Perform the dot product by using parallelism
    double sum = dotproduct_omp_atomic(v1, v2, n);    // parallelism with atomic for the sum
    double sum = dotproduct_omp_critical(v1, v2, n);  // parallelism with critical
    double sum = dotproduct_omp_reduction(v1, v2, n); // parallelism with reduction 
#else                                                 // Perform the dot product sequentially
    double sum = dotproduct(v1, v2, n);
#endif
    printf("sum = %f\n", sum);
    free(v1);
    free(v2);
    return 0;
}
