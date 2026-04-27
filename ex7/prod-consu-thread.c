#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define THREADS_PER_OPERATION 2
#define MAX_TASKS 10

typedef struct {
    int a;
    int b;
    char op;
} Task;

Task buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int total_done = 0;
int produced = 0;

sem_t empty, full, mutex;
FILE *result_file;

void* producer(void* arg) {

    char ops[] = {'+', '-', '*', '/'};

    while (1) {

        sem_wait(&mutex);
        if (produced >= MAX_TASKS) {
            sem_post(&mutex);
            break;
        }
        sem_post(&mutex);

        sem_wait(&empty);
        sem_wait(&mutex);

        Task t;
        t.op = ops[rand() % 4];
        t.a = rand() % 50 + 1;

        if (t.op == '/')
            t.b = rand() % 10 + 1;
        else
            t.b = rand() % 50;

        buffer[in] = t;
        in = (in + 1) % BUFFER_SIZE;

        produced++;

        printf("[Producer] Created: %d %c %d\n", t.a, t.op, t.b);

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {

    char my_op = *(char*)arg;

    while (1) {

        sem_wait(&full);
        sem_wait(&mutex);

        if (total_done >= MAX_TASKS) {
            sem_post(&mutex);
            sem_post(&full);
            pthread_exit(NULL);
        }

        Task t = buffer[out];

        if (t.op == my_op) {

            out = (out + 1) % BUFFER_SIZE;
            total_done++;

            double res = 0;

            if (my_op == '+') res = t.a + t.b;
            if (my_op == '-') res = t.a - t.b;
            if (my_op == '*') res = t.a * t.b;
            if (my_op == '/') res = (double)t.a / t.b;

            printf("[Thread %c] Worked: %d %c %d = %.2f\n",
                   my_op, t.a, t.op, t.b, res);

            fprintf(result_file,"[Thread %c] %d %c %d = %.2f\n", my_op, t.a, t.op, t.b, res);
            fflush(result_file);

            sem_post(&mutex);
            sem_post(&empty);
        }
        else {
            sem_post(&mutex);
            sem_post(&full);
            usleep(100000);
        }
    }
}

int main() {
    int i;
    srand(time(NULL));
    result_file = fopen("result.txt", "w");
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t prod_tid;
    pthread_t add_threads[THREADS_PER_OPERATION];
    pthread_t sub_threads[THREADS_PER_OPERATION];
    pthread_t mul_threads[THREADS_PER_OPERATION];
    pthread_t div_threads[THREADS_PER_OPERATION];

    char op1 = '+', op2 = '-', op3 = '*', op4 = '/';

    pthread_create(&prod_tid, NULL, producer, NULL);

    for (i = 0; i < THREADS_PER_OPERATION; i++) {
        pthread_create(&add_threads[i], NULL, consumer, &op1);
        pthread_create(&sub_threads[i], NULL, consumer, &op2);
        pthread_create(&mul_threads[i], NULL, consumer, &op3);
        pthread_create(&div_threads[i], NULL, consumer, &op4);
    }

    pthread_join(prod_tid, NULL);

    for(i=0;i<THREADS_PER_OPERATION*4;i++)
        sem_post(&full);

    for (i = 0; i < THREADS_PER_OPERATION; i++) {
        pthread_join(add_threads[i], NULL);
        pthread_join(sub_threads[i], NULL);
        pthread_join(mul_threads[i], NULL);
        pthread_join(div_threads[i], NULL);
    }

    printf("\n--- All %d tasks are finished. Program exiting---\n", MAX_TASKS);

    fclose(result_file);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
