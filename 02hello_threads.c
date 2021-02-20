#include <stdio.h> // for printf
#include <omp.h>   // for OpenMP functions

int
main()
{
    printf("only thread number %d here.\n", omp_get_thread_num());

    int thread_id, threads_num;
    #pragma omp parallel private(thread_id, threads_num)
    {
        thread_id   = omp_get_thread_num();
        threads_num = omp_get_num_threads();
        printf("hello from thread %d of %d\n", thread_id, threads_num);
    }
    printf("back to %d thread ", omp_get_num_threads());
    printf("(max is %d).\n", omp_get_max_threads());
}
