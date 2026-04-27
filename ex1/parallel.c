#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

int main() {
    int m, n, p;
    int i, j;
    int r, c, k;
    printf("Enter value of m, n, p: ");
    if (scanf("%d %d %d", &m, &n, &p) != 3) return 1;

    srand(time(NULL));

    int totalSize = (m * n) + (n * p) + (m * p);

    int shmDId = shmget(2061, totalSize * sizeof(int), IPC_CREAT | 0666);
    int shmTId = shmget(2062, m * sizeof(double), IPC_CREAT | 0666);

    if (shmDId < 0 || shmTId < 0) {
        perror("shmget failed");
        exit(1);
    }

    int *data = (int *)shmat(shmDId, NULL, 0);
    double *timeTaken = (double *)shmat(shmTId, NULL, 0);

    int *A = data;
    int *B = data + (m * n);
    int *C = data + (m * n) + (n * p);

    for (i = 0; i < m * n; i++) A[i] = rand() % 5;
    for (i = 0; i < n * p; i++) B[i] = rand() % 5;

    for (r = 0; r < m; r++) {
        if (fork() == 0) {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);

            for (c = 0; c < p; c++) {
                int sum = 0;
                for (k = 0; k < n; k++) {
                    sum += A[r * n + k] * B[k * p + c];
                }
                C[r * p + c] = sum;
            }

            clock_gettime(CLOCK_MONOTONIC, &end);

            uint64_t start_ns = (uint64_t)start.tv_sec * 1000000000ULL + start.tv_nsec;
            uint64_t end_ns = (uint64_t)end.tv_sec * 1000000000ULL + end.tv_nsec;


            timeTaken[r] = (double)(end_ns - start_ns) / 1000000.0;

            exit(0);
        }
    }

    for (i = 0; i < m; i++) wait(NULL);

    double m_time = timeTaken[0];
    for (i = 1; i < m; i++) {
        if (timeTaken[i] > m_time)
            m_time = timeTaken[i];
    }

    printf("\nMaximum Time Taken: %f ms\n", m_time);

    shmdt(data);
    shmdt(timeTaken);
    shmctl(shmDId, IPC_RMID, NULL);
    shmctl(shmTId, IPC_RMID, NULL);

    return 0;
}
