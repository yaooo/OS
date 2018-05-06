#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // number of philosophers
#define runTime 30 // Program run time is currently set to 30 seconds

//conditional variable for all five philosophers
pthread_cond_t cond[N];

bool chopsticks_available[N];
pthread_mutex_t lock;


// test if the neighbors are available
void test(int index) {
	if (!chopsticks_available[index] || !chopsticks_available[(index + 1) % N]) { // if not both chopsticks are available
		pthread_cond_wait(&cond[index], &lock);    // wait for its neighbors
		test(index);
	}
}

// mark the resource in use for the neighbors
void mark_chops_unavailable(int i) {

	// mark the chopsticks in use
	chopsticks_available[i] = false;
	chopsticks_available[(i + 1) % N] = false;

	pthread_cond_signal(&cond[(i + N - 1) % N]);
	pthread_cond_signal(&cond[(i + 1) % N]);
}


void *philosopher(void * param) {
	int *temp = param;
	int i = *temp;

	printf("Philosopher %d is hungry.\n", i);
	sleep(2);

	while (1) {

		pthread_mutex_lock(&lock);

		//wait for the chopsticks
		test(i);
		mark_chops_unavailable(i);

		pthread_mutex_unlock(&lock);

		//EATING, then FINISHED
		printf("Philosopher %d is eating.\n", i);

		sleep(2);

		pthread_mutex_lock(&lock);

		//puts down the chopsticks
		chopsticks_available[i] = true;
		chopsticks_available[(i + 1) % N] = true;
		
		// signal to neighbors
		pthread_cond_signal(&cond[(i + 4) % N]);
		pthread_cond_signal(&cond[(i + 1) % N]);

		//THINKING
		printf("Philosopher %d is thinking.\n", i);
		sleep(2);

		//HUNGRY
		printf("Philosopher %d is hungry.\n", i);
		sleep(2);
		pthread_cond_wait(&cond[i], &lock); //wait for neighbors to eat before

		pthread_mutex_unlock(&lock);

	}
}

int main() {
	pthread_t tid[N];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	for (int i = 0; i < N; i++) {
		pthread_cond_init(&cond[i], NULL);
		chopsticks_available[i] = true;
	}

	pthread_mutex_init(&lock, NULL);

	int starting_id[N];

	for (int i = 0; i < N; i++) {
		starting_id[i] = i;
		pthread_create(&tid[i], &attr, philosopher, (void *) &starting_id[i]);
	}

	// finish running after the main thread wakes up
	sleep(runTime);

	for (int i = 0; i < N; i++) {
		pthread_cancel(tid[i]);
	}

	return 0;
}
