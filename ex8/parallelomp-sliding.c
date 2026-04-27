#include <stdio.h>
#include <omp.h>

int main() {
    int n, k, i, j;
    int thread_count;

    printf("Enter size of array: ");
    scanf("%d", &n);

    int A[n];

    printf("Enter the number of threads:");
    scanf("%d",&thread_count);

    printf("Enter array elements:\n");
    for(i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter window size (k): ");
    scanf("%d", &k);

    int result[n];

    #pragma omp parallel num_threads(thread_count)
    {
        int tid = omp_get_thread_num();
        int first = -1, last = -1;

        #pragma omp for private(j)
        for(i = 0; i <= n - k; i++) {
            if (first == -1) first = i;
            last = i;

            int sum = 0;
            for(j = 0; j < k; j++) {
                sum += A[i + j];
            }
            result[i] = sum;
        }

        #pragma omp critical
        {
            if (first != -1) {
                printf("Thread %d: started at index %d and ended at index %d\n", tid, first, last);
            }
        }
    }




    printf("\nSliding Window Sum:\n");
    for(i = 0; i <= n - k; i++) {
        printf("%d ", result[i]);
    }

    return 0;
}
