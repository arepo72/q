#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 20
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int fill = 0;
int use = 0;
int count = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % BUFFER_SIZE;
    count++;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % BUFFER_SIZE;
    count--;
    return tmp;
}

void *producer(void *arg) {
    int i = 0;
    while (i < MAX) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        printf("Producer produced %d\n", i);
        sem_post(&mutex);
        sem_post(&full);
        i++;
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i = 0;
    while (i < MAX) {
        sem_wait(&full);
        sem_wait(&mutex);
        int tmp = get();
        printf("Consumer consumed %d\n", tmp);
        sem_post(&mutex);
        sem_post(&empty);
        i++;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    printf("Total number of threads Produced: %d\n", MAX);
    
    return 0;
}
