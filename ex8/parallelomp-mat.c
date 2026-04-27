#include <stdio.h>
#include <omp.h>

int main() {
    int n, i, j, temp;
    int thread_count;

    printf("Enter size of matrix (n x n): ");
    scanf("%d", &n);

    printf("Enter the number of threads:");
    scanf("%d",&thread_count);

    omp_set_num_threads(thread_count);

    int A[n][n];

    printf("Enter matrix elements:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    #pragma omp parallel for private(j, temp)
    for(i = 0; i < n; i++) {
        for(j = 0; j < n / 2; j++) {
            temp = A[i][j];
            A[i][j] = A[i][n - j - 1];
            A[i][n - j - 1] = temp;
        }
    }
    printf("\nMirrored Matrix:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
