#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int
main(int argc, char ** argv)
{
    int n = 1000;

    if (argc > 1) {
        n = atoi(argv[1]);
    } else  {
        printf("need one argument\n");
        exit(1);
    }

    int prime;
    int count_prime = 0;

    for (int i = 2; i < n; i++) {
        prime = 1;
        for (int j = 2; j < i - 1; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        count_prime += prime;
    }

    printf("number of primes in [2,%d]:\t %d\n", n, count_prime);
}
