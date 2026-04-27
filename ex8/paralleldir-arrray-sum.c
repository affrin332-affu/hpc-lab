#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 100

int main() {
    int A[SIZE], B[SIZE], C[SIZE];
    int i,thread_count;

    srand(time(0));

    printf("Enter the number of thread count:");
    scanf("%d",&thread_count);

    for(i = 0; i < SIZE; i++) {
        A[i] = rand() % 50;
        B[i] = rand() % 50;
    }
    printf("\nArray A:\n");
    for(i=0;i<10;i++)
    {
       printf("%d ",A[i]);
    }
    printf("\nArray B:\n");
    for(i=0;i<10;i++)
    {
       printf("%d ",B[i]);
    }
    printf("\n");

    #pragma omp parallel for num_threads(thread_count)
    for(i = 0; i < SIZE; i++) {
        int tid = omp_get_thread_num();

        if(i < 10) {
            #pragma omp critical
            printf("Thread %d handles index %d\n", tid, i);
        }

        C[i] = A[i] + B[i];
    }
    printf("\nSample Output:\n");
    for(i = 0; i < 10; i++) {
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
    }

    return 0;
}
