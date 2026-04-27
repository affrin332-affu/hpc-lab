#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX 20
#define THREADS 3

char *buffer[MAX];
char *result_buffer[MAX];

int count = 0;
int result_count = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

const char *dictionary[] = {"apple", "banana", "cat", "dog", "elephant"};
int dict_size = 5;

int spell_check(const char *word) {
    for(int i = 0; i < dict_size; i++) {
        if(strcmp(word, dictionary[i]) == 0) return 1;
    }
    return 0;
}

void* spell_checker(void *arg) {
    long tid = (long)arg;
    while(1) {
        pthread_mutex_lock(&lock);
        while(count == 0) {
            pthread_cond_wait(&cond, &lock);
        }

        char *word = buffer[--count];
        pthread_mutex_unlock(&lock);

        if (word == NULL) break;

        printf("[Thread %ld] Checking: %s\n", tid, word);

        char *result = (char*)malloc(100);
        if(spell_check(word))
            sprintf(result, "%-10s -> CORRECT", word);
        else
            sprintf(result, "%-10s -> INCORRECT", word);

        pthread_mutex_lock(&lock);
        result_buffer[result_count++] = result;
        pthread_mutex_unlock(&lock);

        free(word);
        usleep(100000);
    }
    return NULL;
}

int main() {
    int i;
    pthread_t threads[THREADS];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    for(long i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, spell_checker, (void*)i);
    }

    printf("\n--- Server Dictionary ---\n");
    for(i = 0; i < dict_size; i++) {
        printf("%s\n", dictionary[i]);
    }

    printf("--- Spellcheck Server ---\n");
    printf("Enter words (type 'exit' to finish):\n");

    char input[50];
    while(1) {
        printf("> ");
        scanf("%s", input);
        if(strcmp(input, "exit") == 0) break;

        pthread_mutex_lock(&lock);
        buffer[count++] = strdup(input);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }

    pthread_mutex_lock(&lock);
    for(i = 0; i < THREADS; i++) {
        buffer[count++] = NULL;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);

    for(i = 0; i < THREADS; i++)
    {
       pthread_join(threads[i], NULL);
    }

    printf("\n--- Spell Check Results ---\n");
    for(i = 0; i < result_count; i++) {
        printf("%s\n", result_buffer[i]);
        free(result_buffer[i]);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
