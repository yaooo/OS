#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

void explain_wait_status(pid_t pid, int status){
	if(WIFEXITED(status)){
		printf("Child with PID = %d terminated normally, Exit code: %d\n", pid, WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status)){
		printf("Child with PID = %d terminated by a signal, Exit code: %d\n", pid, WTERMSIG(status));
	}
	else if (WIFSTOPPED(status)){
		printf("Child with PID = %d has been stopped by a signal, Exit code: %d\n", pid, WSTOPSIG(status));
	}
	else{
		printf("%s: Internal error: Unhandled case, PID = %d, Exit code: %d\n", __func__, pid, status);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	int status;
	if(fork() == 0){ 
        pid_t pid = fork();
        sleep(3);
        if(pid == 0){		//child of B(D)
	        printf("I am process D -> PID: %d, ParentPID: %d\n", getpid(), getppid());   
	        printf("D Sleeping now.\n");     	
            sleep(5);
            printf("Process D waiting now.\n");
	        wait(&status);
            explain_wait_status(getpid(), 12);
            exit(12);
        }
        else{				//first child of A(B)
        	printf("I am process B -> PID: %d, ParentPID: %d\n", getpid(), getppid());
        	printf("B Sleeping now.\n");
            sleep(5);
            printf("Process B waiting now.\n");
            wait(&status);
            explain_wait_status(getpid(), 9);
            exit(9);
        }

    }
    else{                   
        if(fork() == 0){    //second child of A(C)
            printf("I am process C -> PID: %d, ParentPID: %d\n", getpid(), getppid());
            printf("C Sleeping now.\n");
        	sleep(10);
        	printf("Process C waiting now.\n");
        	wait(&status);
            explain_wait_status(getpid(), 8);
        	exit(8);
        	
        }
        else{				//parent node A
        	printf("I am process A -> PID: %d, ParentPID: %d\n", getpid(), getppid());
        	printf("A Sleeping now.\n");
            sleep(10);
            printf("Process A waiting now.\n");
            wait(&status);
            explain_wait_status(getpid(), 5);
            exit(5);
        }
    }
	return 0;
}