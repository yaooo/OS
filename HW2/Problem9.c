#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <unistd.h>  // for sleep
#include <semaphore.h> // sem_t semaphore type

sem_t semTeacher;	//semaphore for teachers;
sem_t semChild;		//semaphore for children;
sem_t classroomLockdown;
int ratio = 0; //initializing ratio for user input
int count = 1;

//~~~~~~~~~~~~~~~~~~~~~~~FOR ALL TO USE~~~~~~~~~~~~~~~~~~~~~~~~//

void go_home(){
	printf("Person with ID %d went home.\n", (int)pthread_self());
	pthread_exit(0); //thread is completely done so we need to delete the thread NOTE: MAKE SURE TO DESTROY <SEMS>
}

//~~~~~~~~~~~~~~~~~~~~~~~~TEACHER~~~~~~~~~~~~~~~~~~~~~~~~//

void teacher_enter(){ // counter ++
	sem_post(&semTeacher);	//a teacher entered the classroom, value++
	printf("Teacher entering with ID: %d.\n", (int)pthread_self());
}
void teach(){
	printf("(%d)Teacher teaching...\n", (int) pthread_self());
	sleep(3);	//added waiting time to simulate process time
}
void teacher_exit(float ratioMax){
	//find exit condition: check if there are still teachers left in classroom OR if there are no children left)
	//check if there are enough teachers
	//	if > required, leavue... if < required, LOCKED: teach again and try to exit

	// int sem_getvalue(sem_t * sem, int * value);
	// sem: (Input) A pointer to an initialized unnamed semaphore or an opened named semaphore.
	// value: (Output) A pointer to the integer that contains the value of the semaphore.
	int numberOfTeachersNOW = 0;	// initializing counter for the number of teachers at this time for teacher function
	sem_getvalue(&semTeacher, &numberOfTeachersNOW);
	int numberOfChildrenNOW = 0;	// initializing counter for the number of children at this time for teachers function
	sem_getvalue(&semChild, &numberOfChildrenNOW);
	// printf("There are %d teachers and %d children.\n", numberOfTeachersNOW, numberOfChildrenNOW);
	float checkRatio = 0;
	checkRatio = (float)numberOfChildrenNOW/(numberOfTeachersNOW-1); //calcualte current ratio for if condition

	if (checkRatio <= ratioMax){	//if true, a teacher can leave
		sem_wait(&semTeacher);
		printf("(%d)Teacher exiting.\n", (int)pthread_self());
	}else{	//teacher cannot leave office and continues to teach
		printf("(%d)Teacher wants to leave but cannot. Teaching then will try to exit again soon.\n", (int)pthread_self());
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
	sleep(3);	//added waiting time to simulate process time
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
void verify_compliance(float ratioMax){
	//needs to check if the teacher:child ratio is in check. if GOOD: nothing happens, if BAD: lock it up
	int numberOfTeachersNOWP = 0;	// initializing counter for the number of teachers at this time for parent function
	sem_getvalue(&semTeacher, &numberOfTeachersNOWP);
	int numberOfChildrenNOWP = 0;	// initializing counter for the number of children at this time for parent function
	sem_getvalue(&semChild, &numberOfChildrenNOWP);
	// printf("There are %d teachers and %d children.\n", numberOfTeachersNOWP, numberOfChildrenNOWP);
	float checkRatioP = 0;	//initializing variable

	checkRatioP = (float)numberOfChildrenNOWP/(numberOfTeachersNOWP);	//calcualte current ratio for if condition
	// checks current children to teacher ratio to compare for compliance
	if (checkRatioP <= ratioMax){	//if criteria not met, unlock the classroom
		sem_wait(&classroomLockdown);
		printf("(%d)Parent says everything is fine!\n", (int)pthread_self());
	}else{
		sem_post(&classroomLockdown);	//otherwise, lock the classroom and do no allow teachers to leave
		printf("(%d)Parent says regulations are not met!\n", (int)pthread_self());
	}

}
void parent_exit(){
	printf("(%d)Parent exiting.\n", (int)pthread_self());
	// again - doesnt need counter because there can be many parents
}

//~~~~~~~~~~~~~~~~~~~~~~~People Functions~~~~~~~~~~~~~~~~~~~~~~~~//

void Teacher(){
	for (;;)
	{
		teacher_enter();
		//critical section
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
	printf("Maximum child:teacher ratio: %d\n", R);
	printf("Number of teachers: %d\n", T);
	printf("Number of children: %d\n", C);
	printf("Number of parents: %d\n", P);

	// int sem_init(sem_t *sem, int pshared, unsigned value);
	// function shall initialize the unnamed semaphore referred to by sem
	// If the pshared argument is zero, then the semaphore is shared between threads of the process
	sem_init(&semTeacher, 0, 0);	//these are initialized to 0 value since they are simply there to keep track of how many <people> are entering and leaving **will be used later on: sem_getvalue
	sem_init(&semChild, 0, 0);
	sem_init(&classroomLockdown, 0, 0);
	
	pthread_t teacherThreads[T];	//initializing threads to be created (these will be for thread ID)
	pthread_t childrenThreads[C];
	pthread_t parentThreads[P];

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
		pthread_join(teacherThreads[i], NULL);
	}
	for (int i = 0; i < C; ++i){
		pthread_join(childrenThreads[i], NULL);
	}
	for (int i = 0; i < P; ++i){
		pthread_join(parentThreads[i], NULL);
	}

	sem_destroy(&semTeacher); 
	sem_destroy(&semChild); 
	sem_destroy(&classroomLockdown);

	return 0;
}
