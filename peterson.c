#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 2
int want[2] = {0, 0};
int turn = 0;
int sharedResource = 0;
int processComplete[2] = {0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
void* process(void* arg) {
 int id = *(int*)arg;
 int other = 1 - id;
 want[id] = 1;
 turn = other;
 // Entry section
 while (want[other] && turn == other)
 ;
 // Critical section
 pthread_mutex_lock(&mutex);
 printf("Process %d is accessing the shared resource.\n", id);
 sharedResource++;
 printf("Shared resource value: %d\n", sharedResource);
 pthread_mutex_unlock(&mutex);
 // Exit section
 want[id] = 0;
 // Mark process as complete
 processComplete[id] = 1;
 // Signal the waiting process
 pthread_cond_signal(&condition);
 return NULL;
}
int main() {
 printf("Peterson's Two-Process Synchronization\n");
 pthread_t threads[NUM_THREADS];
 int threadArgs[NUM_THREADS];
 int i;
 for (i = 0; i < NUM_THREADS; i++) {
 threadArgs[i] = i;
 pthread_create(&threads[i], NULL, process, &threadArgs[i]);
 }
 pthread_mutex_lock(&mutex);
 while (!processComplete[0] || !processComplete[1]) {
 pthread_cond_wait(&condition, &mutex);
 }
 pthread_mutex_unlock(&mutex);
 printf("Process 0 finished.\n");
 printf("Process 1 finished.\n");
 printf("Both processes finished execution.\n");
 return 0;
}
