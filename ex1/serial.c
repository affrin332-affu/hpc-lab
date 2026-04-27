#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MAX_VALUE 100

int **c_matrix(int r, int c) {
    int **mat = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        mat[i] = (int *)malloc(c * sizeof(int));
    }
    return mat;
}

void free_m(int **mat, int r) {
    for (int i = 0; i < r; i++) {
        free(mat[i]);
    }
    free(mat);
}

int main() {
    int m, n, p;
    int i, j, k;

    printf("Enter the value of m n p: ");
    if (scanf("%d %d %d", &m, &n, &p) != 3) return 1;

    int **A = c_matrix(m, n);
    int **B = c_matrix(n, p);
    int **C = c_matrix(m, p);

    srand(time(NULL));

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            A[i][j] = (rand() % MAX_VALUE) + 1;

    for (i = 0; i < n; i++)
        for (j = 0; j < p; j++)
            B[i][j] = (rand() % MAX_VALUE) + 1;

    struct timespec start, end;

    // Start timing
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    uint64_t start_ns = (uint64_t)start.tv_sec * 1000000000ULL + start.tv_nsec;
    uint64_t end_ns = (uint64_t)end.tv_sec * 1000000000ULL + end.tv_nsec;
    uint64_t diff_ns = end_ns - start_ns;
    printf("\nTime taken: %f ms\n", (double)diff_ns / 1000000.0);

    free_m(A, m);
    free_m(B, n);
    free_m(C, m);

    return 0;
}
