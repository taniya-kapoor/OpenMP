#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int
main(int argc, char ** argv)
{
    int n = 1000, count_prime = 0;

    if (argc > 1) n = atoi(argv[1]);

    #pragma omp parallel
    {
        double start = omp_get_wtime();

        /*schedule is defined by OMP_SCHEDULE environment variable (or "auto")
         * use: export OMP_SCHEDULE=static,10
         * export OMP_SCHEDULE=dynamic
         * etc. (in command line) to set scheduling policy */
        #pragma omp for reduction(+:count_prime) nowait schedule(runtime)
        for (int i = 2; i < n; i++) {
            int isPrime = 1;
            for (int j = 2; j < i - 1; j++) {
                if (i % j == 0) {
                    isPrime = 0;
                    break;
                }
            }
            count_prime += isPrime;
        } // need "nowait" to NOT wiat here and get correct per-thread timing
        printf("thread %d : %f sec\n", omp_get_thread_num(), omp_get_wtime() - start);
    }

    printf("number of primes in [2,%d]:\t %d\n", n, count_prime);
}
