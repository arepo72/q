#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#define NUM_THREADS 4
bool choosing[NUM_THREADS];
int number[NUM_THREADS];
int max_ticket()
{
 int max = number[0];
 int i;
 for (i = 1; i < NUM_THREADS; ++i)
 {
 if (number[i] > max)
 {
 max = number[i];
 }
 }
 return max;
}
void acquire_lock(int thread_id)
{
 choosing[thread_id] = true;
 number[thread_id] = max_ticket() + 1;
 choosing[thread_id] = false;
 int other;
 for (other = 0; other < NUM_THREADS; ++other)
 {
 while (choosing[other])
 {
 // Wait while other thread chooses its ticket number
 }
 while (number[other] != 0 && (number[other] < number[thread_id] || (number[other] == 
number[thread_id] && other < thread_id)))
 {
 // Wait while other threads with a lower ticket number or an equal number but higher index are in the 
critical section
 }
 }
}
void release_lock(int thread_id)
{
 number[thread_id] = 0;
}
// Example usage
void* thread_function(void* arg)
{
 int thread_id = *(int*)arg;
 acquire_lock(thread_id);
 // Critical section
 printf("Thread %d is in the critical section.\n", thread_id);
 release_lock(thread_id);
 // Remaining section
 printf("Thread %d is in the remaining section.\n", thread_id);
 return NULL;
}
int main()
{
 pthread_t threads[NUM_THREADS];
 int thread_ids[NUM_THREADS];
 int i;
 for (i = 0; i < NUM_THREADS; ++i)
 {
 thread_ids[i] = i;
 pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
 }
 for (i = 0; i < NUM_THREADS; ++i)
 {
 pthread_join(threads[i], NULL);
 }
 return 0;
}
