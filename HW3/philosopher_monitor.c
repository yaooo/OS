#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
enum states {THINKING, HUNGRY, EATING} state[N];
pthread_cond_t cv[N];
pthread_mutex_t m;
int leftHungry[N];
int rightHungry[N];

void * philosopher(void *num);
void pick_up(int);
void put_down(int);
void test(int);

int philosophers[N] = {0, 1, 2, 3, 4};

void *philosopher(void *num) {
	while (1) {
		int *i = num;
		pick_up(*i);
		sleep(1);
		put_down(*i);
	}
}

void pick_up(int i) {
	state[i] = HUNGRY;
	printf("Philosopher %d is HUNGRY\n", i);
	test(i);
	if (state[i] != EATING) {
		pthread_mutex_lock(&m);
		pthread_cond_wait(&cv[i], &m);
		pthread_mutex_unlock(&m);
	}
	rightHungry[(i + 1) % N] = 0;
	leftHungry[(i + 4) % N] = 0;
}


void put_down(int i) {
	state[i] = THINKING;
	printf("Philosopher %d is THINKING \n", i);
	test((i + 4) % N);
	if (state[(i + 4) % N] == HUNGRY) {
		rightHungry[i] = 1;
	}
	test((i + 1) % N);
	if (state[(i + 1) % N] == HUNGRY) {
		leftHungry[i] = 1;
	}
	sleep(2);
}

void test(int i) {
	if (state[(i + 4) % N] != EATING && state[i] == HUNGRY &&  state[ (i + 1) % N] != EATING && !leftHungry[i] && !rightHungry[i]) {
		state[i] = EATING;
		printf("Philosopher %d is EATING\n", i);
		pthread_mutex_lock(&m);
		pthread_cond_signal(&cv[i]);
		pthread_mutex_unlock(&m);
	}
}

int main() {
	int i;
	pthread_t tphilosopher[N];
	printf("\nNote\n**********************************************************************\n");

	printf("Program runs indefinitely, use Ctrl-C or Ctrl-Z (depend on your unix machine) to interrupt and stop when desired.\n");

	printf("***********************************************************************\n\n");

	int all;
	for (all = 0; all < N; all++) {
		pthread_cond_init(&cv[all], NULL);
		leftHungry[all] = 0;
		rightHungry[all] = 0;
	}
	pthread_mutex_init(&m, NULL);

	for (i = 0; i < N; i++) {
		pthread_create(&tphilosopher[i], NULL, philosopher, &philosophers[i]);
		printf("Philosopher %d is THINKING\n", i);
	}

	for (i = 0; i < N; i++) {
		pthread_join(tphilosopher[i], NULL);
	}
}
