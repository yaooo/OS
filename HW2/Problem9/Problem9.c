#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <math.h>

#include "people.h"

void Teacher(){
	for (int i = 0; i < count; ++i)
	{
		teacher_enter();
		//critical section
		teach();
		teacher_exit();
		go_home();
	}
}

void Child(){
	for (int i = 0; i < count; ++i)
	{
		child_enter();
		//critical section
		learn();
		child_exit();
		go_home();
	}
}

void Parent(){
	for (int i = 0; i < count; ++i)
	{
		parent_enter();
		//critical section
		verify_compliance();
		parent_exit();
		go_home();
	}
}


int main(int argc, char const *argv[])
{
	if (argv[0] != 1){
		printf("Error. Please enter a non-negative number.\n");
		return 1;
	}

	int R = argv[0]; //number of children assigned to one teacher

	{
		/* code */
	}
	return 0;
}







