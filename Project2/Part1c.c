#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void handler(int signum, siginfo_t *info, void *extra)
{
    void *ptr_val = info->si_value.sival_ptr;
    int int_val = info->si_value.sival_int;
    printf("Sum=%d\n",int_val);
    printf("SIGUSR1 was used to recieve the data of the variable sum\n");

}

static volatile int keepRunning = 1;
void intHandler(int dummy) {
    keepRunning = 0;
}

static volatile int tap = 6;


void sig_handler(int signo)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    //looks for ctrl-c which has a value of 2
    if (signo == SIGINT){
        tap = 0;
        printf("\nControl C was used on process: ");    
    }
    //looks for ctrl-\ which has a value of 9
    else if (signo == SIGQUIT){
        tap = 1; 
    }
    //looks for ctrl-z 
    else if (signo == SIGTSTP){
        tap = 2;
    }
    else if (signo == SIGTERM){
        tap = 3;
    }

}

int main(){
    signal(SIGINT, intHandler);
    signal(SIGQUIT, sig_handler);
    signal(SIGTSTP, sig_handler);
    signal(SIGTERM, sig_handler);

    int id1 = 0, id2 = 0, id3 = 0;

    int min=0;
    int max=0;
    int sum=0;
    int i, status;
    
    FILE *file1 = fopen("Input_Problem0.txt", "r");
    
    int size = 1;
    int ch = 0;
    
    if(file1 == NULL){
        printf("no numbers in the input file \n");
    }
    
    while(!feof(file1)){
        
        ch = fgetc(file1);
        if(ch == '\n'){
            size++;
        }
    }
    int arrNum[size];
    
    rewind(file1);
    
    for (i = 0; i < size; i++)
    {
        fscanf(file1, "%d", &arrNum[i]);
    }
    
    if(size == 1){
        printf("Max = %d\n", arrNum[0]);
        printf("Min = %d\n", arrNum[0]);
        printf("Sum = %d\n", arrNum[0]);
        return 0;
    }
    

    
    pid_t pid1 = fork();
    if (pid1 == 0){
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        if (tap == 3){
        printf("SIGTERM was used on process: %d\n", getpid());   
        }
        sleep(5);
        wait(&status);
        if(arrNum[0] > arrNum[1]){
            max = arrNum[0];
            min = arrNum[1];
        }else{
            max = arrNum[1];
            min = arrNum[0];
        }
        printf("Hi I am Process 2 and my pid is %d. My Parent's pid is %d\n", getpid(), getppid());
        
        pid_t pid2 = fork();
        if(pid2 == 0){
            wait(&status);
            printf("Hi I am Process 3 and my pid is %d. My Parent's pid is %d\n", getpid(), getppid());
            for(i = 0; i < size; i++){
                
                if(max < arrNum[i]){
                    max = arrNum[i];
                }
            }

            pid_t pid3 = fork();
            if(pid3 == 0){
                wait(&status);
                printf("Hi I am Process 4 and my pid is %d. My Parent's pid is %d\n",getpid(),getppid());
                for(i = 0; i < size; i++){
                
                    if(min > arrNum[i]){
                        min = arrNum[i];
                    }
                }
                printf("Max=%d\n", max);
                printf("Min=%d\n", min);
    
                exit(0);
            } else{
                wait(&status);   
            }
            exit(0);
            
        }else{
            wait(&status);
        }
        for(i = 0; i < size; i++){
            sum += arrNum[i];
        }
        sigqueue(getppid(),SIGUSR1,(const union sigval) sum);

        exit(0);
    }
    else {
        struct sigaction action;
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask,SIGUSR1);
        action.sa_flags = SA_SIGINFO;
        action.sa_mask =mask;
        action.sa_sigaction = &handler;

        if (sigaction(SIGUSR1,&action,NULL)==-1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        //kill(pid1, SIGTERM);

        wait(&status);
    }

    if (keepRunning == 0){
        printf("Control C was used on process: %d\n", getpid());   
    }
    if (tap == 1){
        printf("SIGQUIT was used on process: %d\n", getpid());   
    }
    if (tap == 2){
        printf("SIGTSTP was used on process: %d\n", getpid());   
    }

    fclose (file1);
    
    return 0;
}