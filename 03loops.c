#include <stdio.h>
#include <omp.h>

int
main()
{
    const int n = 1002;
    int count = 0, thread_count = 0;

    #pragma omp parallel firstprivate(thread_count) reduction(+:count)
    {
        #pragma omp for
        for (int j = 0; j < n; ++j) {
            thread_count++;
            count++;
        }
        printf("thread id %d:\t #loop iterations:\t %d\n", omp_get_thread_num(), thread_count);// num_loops);
    }

    printf("Total number of loops:\t %d\n", count);
}
