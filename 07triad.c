#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void
triad(const float * a, const float * b, const float * c, float * d, const size_t n, const size_t m)
{
    for (size_t i = 0; i < n; i++) {
        #pragma omp parallel for
        for (size_t j = 0; j < m; j++) {
            d[j] = c[j] + a[j] * b[j];
        }//<--- this barrier is quite costly.... 
    }
}

int
main(int argc, char ** argv)
{
    size_t m;// aka long unsigned int

    if (argc == 2) {
        m = atoi(argv[1]);
    } else {
        printf("need an argument (vector length)!\n");
        exit(-1);
    }

    unsigned long long int n = (1ULL << 32);
    n /= m;

    float * a, * b, * c, * d;
    a = (float *) malloc(m * sizeof(float));
    b = (float *) malloc(m * sizeof(float));
    c = (float *) malloc(m * sizeof(float));
    d = (float *) malloc(m * sizeof(float));

    for (int i = 0; i < m; i++) {
        a[i] = i + 1.0;
        b[i] = i + 2.0;
        c[i] = i + 3.0;
    }

    struct timespec tstart, tstop;
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    triad(a, b, c, d, n, m);
    clock_gettime(CLOCK_MONOTONIC, &tstop);

    double elapsed = (tstop.tv_sec - tstart.tv_sec) + (tstop.tv_nsec - tstart.tv_nsec) / 1e9;
    printf("m=%lu %llu \t elapsed time: %f \t Mflops: %f\n", m, n, elapsed,
      (2.0 * n * m / elapsed) / (1024.0 * 1024.0));

    free(a);
    free(b);
    free(c);
    free(d);
} /* main */
