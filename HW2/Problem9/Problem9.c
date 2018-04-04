#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <semaphore.h> // for semaphores
#include <math.h>

sem_t semTeacher;	//semaphore for teachers;
sem_t semChild;		//semaphore for children;

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

//sem_init(*semaphore, shared?, value of semaphore)
//~~~~~~~~~~~~~~~~~~~~~~~~TEACHER~~~~~~~~~~~~~~~~~~~~~~~~//

void teacher_enter(){
	sem_init(&semTeacher, 0, )
	printf("Teacher entering with ID: %d.\n", pthread_self());
}
void teach(){

}
void teacher_exit(){

}

//~~~~~~~~~~~~~~~~~~~~~~~~CHILD~~~~~~~~~~~~~~~~~~~~~~~~//

void child_enter(){
	sem_init(&semChild, 0, 0)
	printf("Child entering with ID: %d.\n", pthread_self());
}
void learn(){

}
void child_exit(){

}

//~~~~~~~~~~~~~~~~~~~~~~~PARENT~~~~~~~~~~~~~~~~~~~~~~~~//

void parent_enter(){

}
void verify_compliance(){

}
void parent_exit(){

}

//~~~~~~~~~~~~~~~~~~~~~~~FOR ALL TO USE~~~~~~~~~~~~~~~~~~~~~~~~//

void go_home(){

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int main(int argc, char const *argv[])
{
	if (argv[0] != 4){
		printf("Error. Please follow format: R, # Teachers, # Children, # Parents\n");
		return 1;
	}
	int R = argv[0];	//number of children assigned to one teacher
	int T = argv[1];	//teachers
	int C = argv[2];	//children
	int P = argv[3];	//parents
	pthread_t teacherThreads[T];	//initializing threads to be created (these will be for thread ID)
	pthread_t childrenThreads[C];
	pthread_t parentThreads[P];
	
	//pthread create args: (threadID, attributes: NULL if default, function, function args)
	for (int i = 0; i < T; ++i)
	{
		pthread_create(teacherThreads[i], NULL, Teacher, NULL);
	}
	for (int i = 0; i < C; ++i)
	{
		pthread_create(childrenThreads[i], NULL, Child, NULL);
	}
	for (int i = 0; i < P; ++i)
	{
		pthread_create(parentThreads[i], NULL, Parent, NULL);
	}

	int sem_init(semTeacher, )



	return 0;
}







