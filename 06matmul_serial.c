#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void
initMatrix(float * M, int N)
{
    for (unsigned int i = 0; i < N * N; i++) {
        M[i] = (rand() % 2);
    }
}

void
printMatrix(float * M, int N)
{
    printf("\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%2.3lf ", M[i * N + j]);
        }
        printf("\n");
    }
}

int
isEqual(float * A, float * B, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(A[i * N + j] - B[i * N + j]) > 1e-9)
                return 0;
        }
    }
    return 1;
}

// 1
void
myMatmul_ijk(float * C, const float * A, const float * B, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            float sum = 0;
            for (unsigned int k = 0; k < N; ++k) {
                float a = A[i * N + k];
                float b = B[k * N + j];
                sum += a * b;
            }
            C[i * N + j] = sum;
        }
    }
}

// 2
void
myMatmul_ikj(float * C, const float * A, const float * B, unsigned int N)
{
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int k = 0; k < N; ++k) {
            float a = A[i * N + k];
            for (unsigned int j = 0; j < N; ++j) {
                float b = B[k * N + j];
                C[i * N + j] += a * b;
            }
        }
    }
}

// 3
void
myMatmul_jik(float * C, const float * A, const float * B, unsigned int N)
{
    printf("not implemented\n");
}

// 4
void
myMatmul_jki(float * C, const float * A, const float * B, unsigned int N)
{
    printf("not implemented\n");
}

void
myMatmul_kij(float * C, const float * A, const float * B, unsigned int N)
{
    printf("not implemented\n");
}

void
myMatmul_kji(float * C, const float * A, const float * B, unsigned int N)
{
    printf("not implemented\n");
}

int
main(int argc, char ** argv)
{
    // matrix size (default=3)
    int N = 3;
    if (argc > 1) N = atoi(argv[1]);

    // matmul loop order (default 1)
    int loop_order = 1;
    if (argc > 2) loop_order = atoi(argv[2]);

    // check result (default 0)
    int check_result = 0;
    if (argc > 3) check_result = atoi(argv[3]);

    // allocate and init at 0
    float * A = (float *) calloc(N * N, sizeof(float));
    float * B = (float *) calloc(N * N, sizeof(float));
    float * C = (float *) calloc(N * N, sizeof(float));

    // random init input matrices
    srand(time(NULL));
    initMatrix(A, N);
    initMatrix(B, N);

    // printMatrix(A,N);
    // printMatrix(B,N);

    struct timespec tstart, tend;
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    switch (loop_order) {
        case 1:
            myMatmul_ijk(C, A, B, N);
            break;
        case 2:
            myMatmul_ikj(C, A, B, N);
            break;
        case 3:
            myMatmul_jik(C, A, B, N);
            break;
        case 4:
            myMatmul_jki(C, A, B, N);
            break;
        case 5:
            myMatmul_kij(C, A, B, N);
            break;
        case 6:
            myMatmul_kji(C, A, B, N);
            break;
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);

    //	printMatrix(C,N);

    float seconds = (tend.tv_sec - tstart.tv_sec) + (tend.tv_nsec - tstart.tv_nsec) / 1e9f;
    float Mflops  = (2 * (N * N * N / 1.0e6)) / seconds;
    printf("Seconds: %2.9lf\t MFlops : %2.9lf\n", seconds, Mflops);

    if (check_result) {
        float * D = (float *) calloc(N * N, sizeof(float));
        myMatmul_ijk(D, A, B, N);
        if (isEqual(C, D, N)) {
            printf("check ok\n");
        } else  {
            printf("not ok\n");
        }
        free(D);
    }

    // printMatrix(C,N);
    // printMatrix(D,N);

    free(A);
    free(B);
    free(C);
} /* main */
