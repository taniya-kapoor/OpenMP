#include <stdio.h>  // printf,...
#include <string.h> // memset
#include <stdlib.h> // calloc
#include <math.h>   // atan //link with -lm
#include <time.h>   // clock_gettime,...

void
gauss_seidel(const double hx, const double hy, const int nx, const int ny, const double * f, double * u)
{
    // to be implemented
}

void
redblack_gauss_seidel(const double hx, const double hy, const int nx, const int ny, const double * f, double * u)
{
    // to be implemented
}

void
jacobi(const double hx, const double hy, const int nx, const int ny, const double * f, double * u)
{
    // u_k, u_{k+1}
    double * in, * out;

    in  = (double *) calloc(nx * ny, sizeof(double));
    out = (double *) calloc(nx * ny, sizeof(double));

    memcpy(in, u, nx * ny * sizeof(double));

    // avoid some useless computation in the inner loop...
    const double hxhy = hx * hy;

    // iterations
    for (int k = 0; k < 100000; k++) {
        // jacobi sweep
        for (int i = 1; i < nx - 1; i++) {
            for (int j = 1; j < ny - 1; j++) {
                out[i * nx + j] = 0.25
                  * ((in[i * nx + j + 1]  // right
                  + in[i * nx + j - 1]    // left
                  + in[(i - 1) * nx + j]  // up
                  + in[(i + 1) * nx + j]) // down
                  - hxhy * f[i * nx + j]);
            }
        }

        for (int i = 1; i < nx - 1; i++) {
            for (int j = 1; j < ny - 1; j++) {
                in[i * nx + j] = out[i * nx + j];
            }
        }
    }

    memcpy(u, in, nx * ny * sizeof(double));
    free(in);
    free(out);
} /* jacobi */

double
norm2(const double * vec, int len)
{
    double n2 = 0.0;

    for (size_t i = 0; i < len; i++) {
        n2 += vec[i] * vec[i];
    }
    return sqrt(n2);
}

double
computeErr(const double * u, const double * uex, const int nx, const int ny)
{
    double err, tmp = 0;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            tmp += pow(u[i * nx + j] - uex[i * nx + j], 2.0);
        }
    }
    return sqrt(tmp) / norm2(uex, nx * ny);
}

int
main(int argc, char ** argv)
{
    const int n  = 250;// nb points in each dimension
    const int nx = n;
    const int ny = n;

    // alloc and zero-init
    double * u = (double *) calloc(nx * ny, sizeof(double));

    // rhs and exact solution
    double * f   = (double *) malloc(nx * ny * sizeof(double));
    double * uex = (double *) malloc(nx * ny * sizeof(double));

    double * x, * y;// grid

    x = (double *) malloc(nx * sizeof(double));
    y = (double *) malloc(ny * sizeof(double));

    // ========================================
    double hx, hy;
    const double pi = 4.0 * atan(1.0);
    // printf("%15.16f\n",pi);

    // compute mesh
    hx = 1.0 / (nx - 1);
    for (int i = 0; i < nx; i++) x[i] = i * hx;
    hy = 1.0 / (ny - 1);
    for (int i = 0; i < ny; i++) y[i] = i * hy;

    printf("Mesh size:\t %dx%d / %f %f\n", nx, ny, hx, hy);

    // compute rhs and exact sol
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            uex[i * nx + j] = sin(pi * x[i]) * sin(pi * y[j]);
            f[i * nx + j]   = -2.0 * pi * pi * uex[i * nx + j];
        }
    }

    // solve linear problem
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);
    jacobi(hx, hy, nx, ny, f, u);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    // compute error
    double err = computeErr(u, uex, nx, ny);
    printf("Error:\t %f\n", err);

    double elapsed = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed:\t %2.10f\n", elapsed);

    /* write solution, exact solution and local error in result file
     * to visualize with gnuplot ... start gnuplot and type ...
     * set pm3d; set hidden3d;
     * splot 'results_jacobi.dat' using 1:2:3 (computed solution)
     * splot 'results_jacobi.dat' using 1:2:4 (exact solution)
     * splot 'results_jacobi.dat' using 1:2:5 (local error)*/
    FILE * fd;
    fd = fopen("./results_jacobi.dat", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(fd, "%f\t%f\t%f\t%f\t%f\n", x[i], y[j], u[i * nx + j], uex[i * nx + j],
              u[i * nx + j] - uex[i * nx + j]);
        }
        fprintf(fd, "\n");
    }
    fclose(fd);

    // free memory
    free(u);
    free(f);
    free(uex);
    free(x);
    free(y);
} /* main */
