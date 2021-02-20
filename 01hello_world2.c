#include <stdio.h>

int
main()
{
    #pragma omp parallel
    { // this is ...
        printf("hello ");
        printf("openmp!\n");
    } // ... a structured block
}
