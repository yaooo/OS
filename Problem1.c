#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void returnTree(){
	
}



int main(int argc, char const *argv[])
{
    if(fork() == 0){
        pid_t pid = fork();
        printf("PID: %d, ParentPID: %d\n", getpid()-27000, getppid()-27000);
        printf("going to sleep A\n");
        sleep(4);
        if(pid == 0){
        	exit(0);
        }
        else{
            wait(NULL);
            	exit(0);
        }
    }
    else{
        if(fork() == 0){
            printf("PID: %d, ParentPID: %d\n", getpid()-27000, getppid()-27000);
            printf("going to sleep B\n");
        	sleep(4);
        	exit(0);
        }
        else{
            wait(NULL);
            exit(0);
        }
        printf("PID: %d, ParentPID: %d\n", getpid()-27000, getppid()-27000);
		printf("going to sleep C\n");
        sleep(4);
    }
    return 0;
}