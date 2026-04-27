#include <stdio.h>
#include <omp.h>

int main() {
    int n, target, threads, count = 0;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter target value: ");
    scanf("%d", &target);

    // Added input for thread count
    printf("Enter number of threads: ");
    scanf("%d", &threads);

    #pragma omp parallel for num_threads(threads) reduction(+:count)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();

        if (arr[i] < target) {
            printf("Thread %d: number [%d] is less than target %d\n", tid, arr[i], target);
            count++;
        } else {
            printf("Thread %d: number [%d] is not less than target %d\n", tid, arr[i], target);
        }
    }

    printf("\nTotal count: %d\n", count);

    return 0;
}

[23bcs062@mepcolinux ex9]$g++ -fopenmp col.c -o col
[23bcs062@mepcolinux ex9]$./col
Enter number of elements: 5
Enter 5 elements: 1 3 6 8 4
Enter target value: 4
Enter number of threads: 2
Thread 0: number [1] is less than target 4
Thread 0: number [3] is less than target 4
Thread 1: number [8] is not less than target 4
Thread 1: number [4] is not less than target 4
Thread 0: number [6] is not less than target 4

Total count: 2
[23bcs062@mepcolinux ex9]$cat max.c
#include <stdio.h>
#include <omp.h>

int main() {
    int n, threads;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter number of threads: ");
    scanf("%d", &threads);

    int max_val = arr[0];

    #pragma omp parallel for num_threads(threads) reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        int tid = omp_get_thread_num();

        if (arr[i] > max_val) {
            max_val = arr[i];
        }

        printf("Thread %d: Checks value  [%d]\n", tid, arr[i]);
    }

    printf("\nThe maximum element is: %d\n", max_val);

    return 0;
}
