#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#ifndef _OPENMP
#define omp_get_thread_num() 0
#endif
int main(void)
{
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf(" [%d ]\tHello, World !\n", tid);
    }

    return EXIT_SUCCESS;
}
