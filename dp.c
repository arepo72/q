#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_PHILOSOPHERS 3
#define MAX_MEALS 3
pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
int philosopher_ids[NUM_PHILOSOPHERS];
int meals_eaten[NUM_PHILOSOPHERS] = {0};
void think(int philosopher_id)
{
 printf("Philosopher %d is thinking.\n", philosopher_id);
 sleep(rand() % 3 + 1); // Sleep for a random time to simulate thinking
}
void eat(int philosopher_id)
{
 printf("Philosopher %d is eating.\n", philosopher_id);
 sleep(rand() % 3 + 1); // Sleep for a random time to simulate eating
}
void* philosopher_function(void* arg)
{
 int philosopher_id = *(int*)arg;
 int left_fork = philosopher_id;
 int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;
 int meals = 0;
 while (meals < MAX_MEALS)
 {
 think(philosopher_id);
 // Acquire the left fork
 pthread_mutex_lock(&forks[left_fork]);
 // Acquire the right fork
 pthread_mutex_lock(&forks[right_fork]);
 eat(philosopher_id);
 // Release the forks
 pthread_mutex_unlock(&forks[left_fork]);
 pthread_mutex_unlock(&forks[right_fork]);
 meals++;
 meals_eaten[philosopher_id]++;
 }
 return NULL;
}
int main()
{
 srand(time(NULL));
 // Initialize forks
 int i;
 for (i = 0; i < NUM_PHILOSOPHERS; ++i)
 {
 pthread_mutex_init(&forks[i], NULL);
 }
 // Create philosopher threads
 for (i = 0; i < NUM_PHILOSOPHERS; ++i)
 {
 philosopher_ids[i] = i;
 pthread_create(&philosophers[i], NULL, philosopher_function, &philosopher_ids[i]);
 }
 // Wait for philosopher threads to finish
 for (i = 0; i < NUM_PHILOSOPHERS; ++i)
 {
 pthread_join(philosophers[i], NULL);
 }
 // Destroy forks
 for (i = 0; i < NUM_PHILOSOPHERS; ++i)
 {
 pthread_mutex_destroy(&forks[i]);
 }
 // Print meals eaten by each philosopher
 printf("Meals Eaten:\n");
 for (i = 0; i < NUM_PHILOSOPHERS; ++i)
 {
 printf("Philosopher %d: %d\n", i, meals_eaten[i]);
 }
 return 0;
}
