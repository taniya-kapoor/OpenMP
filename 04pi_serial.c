#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int
main(int argc, char ** argv)
{
    long long int N = 1ULL << 31;

    if (argc > 1) N = atoll(argv[1]);

    struct timespec tstart, tend;
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    double sum = 0.0;
    int sign   = 1;

    for (long long int i = 0; i < N; i++) {
        // sign = pow(-1.0,i);
        sign = (i & 1ULL) ? (-1) : 1;
        sum += sign / (2.0 * i + 1);
    }
    printf("PI:\t %1.15lf\n", 4 * sum);

    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("Time(s):\t %2.9lf\n",
      (tend.tv_sec - tstart.tv_sec) + (tend.tv_nsec - tstart.tv_nsec)
      / 1e9f);
}
