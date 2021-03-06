#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <unistd.h>  // for sleep
#include <semaphore.h> // sem_t semaphore type

sem_t access;	//binary semaphore - only allows access to either the employee or the cleaner (mutex condidtion)

int N; //N = number of <employees>
sem_t keychainEmployee;	//initialize keychain value to the number of employees (N)
int M; //M = number of <employees>
sem_t keychainCleaner;	//initialize keychain value to the number of employees (N)

//wait: to check if worker can go into the office space (increment semaphore count if successful)
//signal: open up a space in office (decrement semaphore count if successful)

void employee(){
	while(true){
		wait(&access);		//switches access to ONLY employees if cleaners are not working at the moment
		wait(&keychainEmployee);	//takes one key from the keychain (decrements the value)
		working_window();
		signal(&keychainEmployee)	//surrenders key back to keychain (increment the value)
		int numberOfKeysTaken = 0; //initialize some counter
		sem_getvalue(&keychain, &numberOfKeysLeft)
		if (numberOfKeysLeft == N)	//if there is an available key for every employee, that means there are no employees left
		{
			signal(&access);	//give up access to room. The next worker in queue will get access for next iteration
		}
		break_time();
	}
}

void cleaner(){
	while(true){
		wait(&access);	//switches access to ONLY cleaners if employees are not working at the moment
		wait(&keychainCleaner);	//takes one key from the keychain (decrements the value)		
		clean_office();
		signal(&keychainCleaner);	//surrenders key back to keychain for cleaners
		int numberOfKeysTaken = 0; //initialize some counter
		sem_getvalue(&keychainCleaner, &numberOfKeysLeft)
		if (numberOfKeysLeft == M)
		{
			signal(&access);
		}
		break_time();
	}
}

void main(int argc, char const *argv[])
{
	sem_init(&access, 0, 1);
	sem_init(&keychainEmployee, 0, N);
	sem_init(&keychainCleaner, 0, M);
	pthread_t employeeThreads[N];
	pthread_t cleanerThreads[M];
	for (int i = 0; i < N; ++i){
		pthread_create(&employeeThreads[i], NULL, (void*)employee, NULL)
	}
	for (int i = 0; i < M; ++i){
		pthread_create(&cleanerThreads[i], NULL, (void*)cleaner, NULL)
	}
	for (int i = 0; i < N; ++i){
		pthread_join(&employeeThreads[i], NULL)
	}
	for (int i = 0; i < M; ++i){
		pthread_join(&cleanerThreads[i], NULL)
	}
	sem_destroy(&access);
	sem_destroy(&keychainEmployee);
	sem_destroy(&keychainCleaner);
	
	return 0;
}