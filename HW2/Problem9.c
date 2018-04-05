#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <semaphore.h> // for semaphores
#include <math.h>

sem_t semTeacher;	//semaphore for teachers;
sem_t semChild;		//semaphore for children;
int count = 1;


//sem_init(*semaphore, shared?, value of semaphore)
//~~~~~~~~~~~~~~~~~~~~~~~~TEACHER~~~~~~~~~~~~~~~~~~~~~~~~//

void teacher_enter(){
	int sem_post(&semTeacher);	//a teacher entered the classroom, value++
	printf("Teacher entering with ID: %d.\n", (int)pthread_self());
}
void teach(){
	
	printf("(%d)Teacher teaching.\n", (int) pthread_self());
}
void teacher_exit(){
	//find exit condition: check if there are still teachers left in classroom OR if there are no children left)
	//check if there are enough teachers

	// int sem_getvalue(sem_t * sem, int * value);
	// sem: (Input) A pointer to an initialized unnamed semaphore or an opened named semaphore.
	// value: (Output) A pointer to the integer that contains the value of the semaphore.
	if (sem_getValue(&semTeacher) > 0)	//if there is more than
	{
		/* code */
	}
	int sem_wait(&semTeacher);	//opposite of entering, value-- (MUST BE WITHIN REASON!)
}

//~~~~~~~~~~~~~~~~~~~~~~~~CHILD~~~~~~~~~~~~~~~~~~~~~~~~//

void child_enter(){
	//we need counter++ (semaphore unlocked)
	int sem_post(&semChild)
	printf("Child entering with ID: %d.\n", (int)pthread_self());
}
void learn(){
	printf("(%d)Child learning.\n", (int)pthread_self());
}
void child_exit(){
	//counter--
	sem_wait(&semChild)
	printf("(%d)Child exiting.\n", (int)pthread_self());

}

//~~~~~~~~~~~~~~~~~~~~~~~PARENT~~~~~~~~~~~~~~~~~~~~~~~~//

void parent_enter(){
	printf("Parent entering with ID: %d.\n", (int)pthread_self());

}
void verify_compliance(){

}
void parent_exit(){
	printf("(%d)Parent exiting.\n", (int)pthread_self());
}

//~~~~~~~~~~~~~~~~~~~~~~~FOR ALL TO USE~~~~~~~~~~~~~~~~~~~~~~~~//

void go_home(){
	printf("Person with ID %d went home.\n", (int)pthread_self());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void Teacher(){
	for (;;)
	{
		teacher_enter();
		//critical section
		printf("Teacher in critical section\n");
		teach();
		teacher_exit();
		go_home();
	}
}

void Child(){
	for (;;)
	{
		child_enter();
		//critical section
		printf("Child in critical section\n");
		learn();
		child_exit();
		go_home();
	}
}

void Parent(){
	for (;;)
	{
		parent_enter();
		//critical section
		printf("Parent in critical section\n");
		verify_compliance();
		parent_exit();
		go_home();
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 4){
		printf("%d\n", argc);
		printf("Error. Please follow format: R, # Teachers, # Children, # Parents\n");
		return 1;
	}
	int R = atoi(argv[0]);	//number of children assigned to one teacher
	int T = atoi(argv[1]);	//teachers
	int C = atoi(argv[2]);	//children
	int P = atoi(argv[3]);	//parents
	pthread_t teacherThreads[T];	//initializing threads to be created (these will be for thread ID)
	pthread_t childrenThreads[C];
	pthread_t parentThreads[P];
	// int sem_init(sem_t *sem, int pshared, unsigned value);
	// function shall initialize the unnamed semaphore referred to by sem
	// If the pshared argument is zero, then the semaphore is shared between threads of the process
	sem_init(&semTeacher, 0, 0);
	
	//pthread create args: (threadID, attributes: NULL if default, function, function args)
	for (int i = 0; i < T; ++i)	//for every teacher, child, and parent, create a thread for their function
	{
		pthread_create(&teacherThreads[i], NULL, (void*)Teacher, NULL);
	}
	for (int i = 0; i < C; ++i)
	{
		pthread_create(&childrenThreads[i], NULL, (void*)Child, NULL);
	}
	for (int i = 0; i < P; ++i)
	{
		pthread_create(&parentThreads[i], NULL, (void*)Parent, NULL);
	}

	// int sem_init(semTeacher, )



	return 0;
}
