#include <stdio.h>

int
main()
{
    #pragma omp parallel
    printf("hello openmp!\n");
}
