#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 20

int arr[MAX];
int partial_sums[MAX];
int n, num_threads;

void *calculateSum(void *arg) {
    long id = (long)arg;
    int i;
    int size=n/num_threads;
    int start = id * size;
    int end = (id == num_threads - 1) ? n : (start + size);

    printf("\nThread %ld handling elements: ", id);

    partial_sums[id] = 0;
    for (i = start; i < end; i++) {
        printf("%d ", arr[i]);
        partial_sums[id] += arr[i];
    }
    printf("\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    int i;
    num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];

    printf("Enter no.of elements: ");
    scanf("%d", &n);

    printf("Enter %d elements:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, calculateSum, (void *)(long)i);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_sum = 0;
    for (i = 0; i < num_threads; i++) {
        total_sum += partial_sums[i];
    }

    printf("\nFinal Sum = %d\n", total_sum);
    return 0;
}
