#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <unistd.h> 
#include <semaphore.h> // for semaphores
#include <math.h>

sem_t semTeacher;	//semaphore for teachers;
sem_t semChild;		//semaphore for children;
sem_t classroomLockdown;
int ratio = 0;
int count = 1;


//~~~~~~~~~~~~~~~~~~~~~~~~TEACHER~~~~~~~~~~~~~~~~~~~~~~~~//

void teacher_enter(){ // counter ++
	sem_post(&semTeacher);	//a teacher entered the classroom, value++
	printf("Teacher entering with ID: %d.\n", (int)pthread_self());
}
void teach(){
	printf("(%d)Teacher teaching...\n", (int) pthread_self());
	sleep(1);
}
void teacher_exit(int ratioMax){
	//find exit condition: check if there are still teachers left in classroom OR if there are no children left)
	//check if there are enough teachers
	//	if > required, leavue... if < required, LOCKED: teach again and try to exit

	// int sem_getvalue(sem_t * sem, int * value);
	// sem: (Input) A pointer to an initialized unnamed semaphore or an opened named semaphore.
	// value: (Output) A pointer to the integer that contains the value of the semaphore.
	int numberOfTeachersNOW = 0;
	sem_getvalue(&semTeacher, &numberOfTeachersNOW);
	int numberOfChildrenNOW = 0;
	sem_getvalue(&semChild, &numberOfChildrenNOW);
	float checkRatio = 0;
	if (numberOfTeachersNOW > 0)
	{
		float checkRatio = numberOfChildrenNOW/numberOfTeachersNOW;	//compare this value with R
	}else if (numberOfChildrenNOW == 0) //no children left, can leave now
	{
		sem_wait(&semTeacher);
		printf("(%d)Teacher exiting.\n", (int)pthread_self());
	}
	if (checkRatio < ratioMax){	//if true, a teacher can leave
		sem_wait(&semTeacher);
		printf("(%d)Teacher exiting.\n", (int)pthread_self());
	}else{	//teacher cannot leave office and continues to teach
		printf("(%d)Teacher wants to leave but cannot. Will try to leave again soon.\n", (int)pthread_self());
		teach(); //goes back to teaching
		teacher_exit(ratioMax);
	}
	// int sem_wait(&semTeacher);	//opposite of entering, value-- (MUST BE WITHIN REASONAL CONDITIONS!)
}

//~~~~~~~~~~~~~~~~~~~~~~~~CHILD~~~~~~~~~~~~~~~~~~~~~~~~//

void child_enter(){
	//we need counter++ (semaphore unlocked)
	sem_post(&semChild);
	printf("Child entering with ID: %d.\n", (int)pthread_self());
}
void learn(){
	printf("(%d)Child learning...\n", (int)pthread_self());
	sleep(1);
}
void child_exit(){
	//counter--
	sem_wait(&semChild);
	printf("(%d)Child exiting.\n", (int)pthread_self());

}

//~~~~~~~~~~~~~~~~~~~~~~~PARENT~~~~~~~~~~~~~~~~~~~~~~~~//

void parent_enter(){
	printf("Parent entering with ID: %d.\n", (int)pthread_self());
	//dont need counter(semaphore) for parents because there can be many parents without bounds
}
void verify_compliance(int ratioMax){
	//needs to check if the teacher:child ratio is in check. if GOOD: nothing happens, if BAD: lock it up
	int numberOfTeachersNOW = 0;
	sem_getvalue(&semTeacher, &numberOfTeachersNOW);
	int numberOfChildrenNOW = 0;
	sem_getvalue(&semChild, &numberOfChildrenNOW);
	float checkRatio = 0;
	if (numberOfTeachersNOW > 0)
	{
		float checkRatio = numberOfChildrenNOW/numberOfTeachersNOW;	//compare this value with R
	}
	if (checkRatio < ratioMax){
		sem_wait(&classroomLockdown);
		printf("(%d)Parent says everything is fine!\n", (int)pthread_self());
	}else{
		sem_post(&classroomLockdown);
		printf("(%d)Parent says regulations are not met!\n", (int)pthread_self());
	}

}
void parent_exit(){
	printf("(%d)Parent exiting.\n", (int)pthread_self());
	// again - doesnt need counter because there can be many parents
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
		// printf("(%d)Teacher in critical section\n", (int)pthread_self());
		teach();
		teacher_exit(ratio);
		go_home();
	}
}

void Child(){
	for (;;)
	{
		child_enter();
		//critical section
		// printf("(%d)Child in critical section\n", (int)pthread_self());
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
		// printf("(%d)Parent in critical section\n", (int)pthread_self());
		verify_compliance(ratio);
		parent_exit();
		go_home();
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 5){
		printf("%d\n", argc);
		printf("Error. Please follow format: R, # Teachers, # Children, # Parents\n");
		return 1;
	}
	int R = atoi(argv[1]);	//max number of children assigned to one teacher
	ratio += R; 			//change global variable value
	int T = atoi(argv[2]);	//teachers
	int C = atoi(argv[3]);	//children
	int P = atoi(argv[4]);	//parents
	printf("%d %d %d %d \n", (R, T, C, P));
	pthread_t teacherThreads[T];	//initializing threads to be created (these will be for thread ID)
	pthread_t childrenThreads[C];
	pthread_t parentThreads[P];
	// int sem_init(sem_t *sem, int pshared, unsigned value);
	// function shall initialize the unnamed semaphore referred to by sem
	// If the pshared argument is zero, then the semaphore is shared between threads of the process
	sem_init(&semTeacher, 0, 0);
	sem_init(&semChild, 0, 0);
	sem_init(&classroomLockdown, 0, 1);
	
	//pthread create args: (threadID, attributes: NULL if default, function, function args)
	for (int i = 0; i < T; ++i){	//for every teacher, child, and parent, create a thread for their function
		pthread_create(&teacherThreads[i], NULL, (void*)Teacher, NULL);
	}
	for (int i = 0; i < C; ++i){
		pthread_create(&childrenThreads[i], NULL, (void*)Child, NULL);
	}
	for (int i = 0; i < P; ++i){
		pthread_create(&parentThreads[i], NULL, (void*)Parent, NULL);
	}
	for (int i = 0; i < T; ++i){	//for every teacher, child, and parent, create a thread for their function
		pthread_join(&teacherThreads[i], NULL);
	}
	for (int i = 0; i < C; ++i){
		pthread_join(&childrenThreads[i], NULL);
	}
	for (int i = 0; i < P; ++i){
		pthread_join(&parentThreads[i], NULL);
	}

	sem_destroy(&semTeacher); sem_destroy(&semChild); sem_destroy(&classroomLockdown);



	return 0;
}
