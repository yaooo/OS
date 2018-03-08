#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signalHandler(int signum){
	printf("Signal: %d\n", signum);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, signalHandler);
	if(fork() == 0){ 
        pid_t pid = fork();
        sleep(3);
        if(pid == 0){		//child of B(D)
	        printf("I am process D -> PID: %d, ParentPID: %d\n D Sleeping now.\n", getpid(), getppid());        	
            // sleep(5);
            
            wait(NULL);
            printf("Process D: Terminating with exit code: 12\n");
            exit(12);
        }
        else{				//first child of A(B)
        	printf("I am process B -> PID: %d, ParentPID: %d\n B Sleeping now.\n", getpid(), getppid());
            sleep(5);
            wait(NULL);
            printf("Process B: Terminating with exit code: 9\n");            
            exit(9);
        }
    }
    else{                   
        if(fork() == 0){    //second child of A(C)
            printf("I am process C -> PID: %d, ParentPID: %d\n  C Sleeping now.\n", getpid(), getppid());
        	sleep(5);
        	wait(NULL);
            
            printf("Process C: Terminating with exit code: 8\n");        	
        	exit(8);
        	
        }
        else{				//NODE A
        	printf("I am process A -> PID: %d, ParentPID: %d\n A Sleeping now.\n", getpid(), getppid());
            sleep(10);
            
            wait(NULL);
            printf("Process A: Terminating with exit code: 5\n");            
            exit(5);
        }
        sleep(3);
    }



	return 0;
   
    

	// pid_t pid1 = fork();
	// if (pid1 < 0)
	// {
	// 	perror("Error: ");
	// }
	// else if (pid1 == 0){
	// 	//child process
	// 	printf("I am: %d| my Parent is: %d\n", getpid(), getppid());
	// 	spawnChild();
	// }
	// else{
	// 	//process of parent of pid1
	// 	printf("I am: %d| my Parent is: %d\n", getpid(), getppid());
	// 	spawnChild();
	// }

}