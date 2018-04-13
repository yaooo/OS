#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <unistd.h>  // for sleep
#include <semaphore.h> // sem_t semaphore type

sem_t access;

//wait: to check if worker can go into the office space (increment semaphore value if successful)
//signal: open up a space in office (decrement semaphore value if successful)
sem_init(&access, 0, 1);

void employee(){
	while(true){
		wait(&access);	//checks to see if the office is open (no cleaners inside)
						//if cleaner is inside, block the employee from entering
						//note: as long as an employee has access, more exployees can join in
		working_window();
		signal(&access);
		break_time();
	}
}

void cleaner(){
	while(true){
		wait(&access);	//checks to see if the office is open (no employees inside)
						//if employee is inside, block the cleaner from entering
						//enter queue to be the next one to enter office
		clean_office();
		signal(&access);
		break_time();
	}
}

void main(int argc, char const *argv[])
{
	sem_init(&access, 0, 1);
	pthread_create(&access, NULL, (void*)employee, NULL);
	pthread_create(&access, NULL, (void*)cleaner, NULL);

	pthread_join(&access, NULL)

	sem_destroy(&access);	
	return 0;
}
