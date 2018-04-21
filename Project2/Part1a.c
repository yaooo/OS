#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>


int num[1000000];
int len = 0;
clock_t startTime;
double elapsedTime;
int numberOfProcesses = 0;

// min, max, sum functions
int min(int *x, int len) { //op 1
    int mintemp = x[0], i;
    for (i = 1;i < len;i++) {
        if(x[i] < mintemp) {
            mintemp = x[i];
        }
    }
    return mintemp;
}
int max(int *y, int len) { //op 2
    int maxtemp = y[0], j;
    for (j = 1;j < len;j++) {
        if(y[j] > maxtemp) {
            maxtemp = y[j];
        }
    }
    return maxtemp;
}
int sum(int *z, int len) { //op 3
    int sumtemp = 0, k;
    for (k = 0;k < len;k++) {
        sumtemp = sumtemp + z[k];
    }
    return sumtemp;
}

void signalHandler(int signalNumber, siginfo_t *information, void *context){
    int value = information->si_value.sival_int;
    printf("\bThe calculated sum is %d. Recieved Signal: %d\n", value, signalNumber);
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Error. Please follow the instruction of executing this program in ReadMe.\n");
        printf("Do ./<executable> <inputFileName> <outputFileName>\n");
        return 1;
    }

    char *inputFileName = argv[1];
    char *OutputFileName = argv[2];
    
    if(access(inputFileName, F_OK ) != -1){      //check if the file exists
        FILE *fp = fopen(inputFileName,"r");
        while(1) {
            int goodData = fscanf(fp, "%d", &num[len]);
            len++;  //counts number of elements
            if (feof(fp)){
                break;
            }
            if(goodData == EOF){
                printf("error");
                return 1;
            }
        }
        fclose(fp);
    } else {
        printf("error\n");
        return 1;
    }
    //at this point num array has all numbers and there are 'len' number of elements

    struct sigaction action;
    action.sa_sigaction = &signalHandler;
    //The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler.
    action.sa_flags = SA_SIGINFO;

    pid_t root, p1MIN, p2MAX, p3SUM; //p1 and p2
    root = getpid();
    printf("Root PID: %d\n", root);
    
    p1MIN = fork();
    if (p1MIN == 0){ //inside p1min (c)
        wait(NULL);
        printf("I am: %d. Parent: %d\n", getpid(), getppid());
        
        const union sigval sumVal = {sum(num, len)};
        
        sigqueue(getppid(), SIGUSR1, sumVal);
        p2MAX = fork();
        if (p2MAX == 0){ //inside p2MAX (c)
            wait(NULL);
            int maxVal = max(num,len);
            printf("I am: %d. Parent: %d. The calculated max from my process is: %d\n", getpid(), getppid(), maxVal);
            p3SUM = fork();
            if (p3SUM == 0){ //inside p3SUM (c)
                wait(NULL);
                int minVal = min(num,len);
                printf("I am: %d. Parent: %d. The calculated min from my process is: %d\n", getpid(), getppid(), minVal);
            }
        }
    }
    else{
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask,SIGUSR1);
        action.sa_mask =mask;
        if (sigaction(SIGUSR1,&action,NULL)==-1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }

    // numberOfProcesses++;
    // for (int i = 0; i < 3; ++i)
    // {
    //     numberOfProcesses++;
    //     p1 = fork();
    //     if (p1 != 0)
    //     {
    //         numberOfProcesses++;
    //         p2 = fork();
    //     }
    //     if (p1 != 0 && p2 != 0)
    //     {
    //         numberOfProcesses++;
    //         break;
    //     }
    //     // printf("I am: %d. Parent: %d\n", getpid(), getppid());
    //     wait(0);
    // printf("I am: %d. Parent: %d\n", getpid(), getppid());
    // sigqueue(getpid(), SIGUSR1, (const union sigval) sum(num, len));
    // startTime = clock();
    // FILE *out = fopen(OutputFileName, "w+");
    // printf("hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
    // printf("Max=%d\n", max(num, len)); // pass array and len
    // printf("Min=%d\n", min(num, len));
    // printf("Sum=%d\n", sum(num, len));
    
    // fclose(out);
    // //printf("Output file is generated for problem 0 part a.\n");

    // elapsedTime = (clock() - startTime);
    // printf("Total time: %0.3f ticks from Problem 0 part a.(1 tick = 1/1000000s)\n", elapsedTime);
    wait(&root);
    return 0;
}



