#include "problem4.h"
extern int NEGLIM;

void *safeRealloc(void *ptr, int *currSize, int dataSize) {
	int newSize = *currSize + MAXFILELEN;
	ptr = realloc(ptr, newSize * dataSize);
	*currSize = newSize;
}

void handler(int sig) { 
	if (sig == SIGALRM) {
		int processID = getpid();
		int parentID = getppid();
		printf("process %d, child of %d had an error\n", processID, parentID);
		exit(1); 
	}
	else {
		printf("this message should not be printed\n");
	}
}

void ERROR(char *error) {
	int processID = getpid();
	printf("%s\n",error);
	if (signal(SIGALRM, handler)== SIG_ERR) {
		printf("Unable to install SIGALRM handler\n");
		exit(1);
	}
	kill(processID, SIGALRM);
}
