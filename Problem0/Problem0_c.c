#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <time.h>


int num[1000000];
int len = 0;
clock_t startTime;
double elapsedTime;

// min, max, sum functions
int min(int *x, int begin, int end) {
    int mintemp = x[0];
    for (int i = begin;i < end; i++) {
        if(x[i] < mintemp) {
            mintemp = x[i];
        }
    }
    return mintemp;
}
int max(int *x, int begin, int end) {
    int maxtemp = x[0];
    for (int j = begin; j < end; j++) {
        if(x[j] > maxtemp) {
            maxtemp = x[j];
        }
    }
    return maxtemp;
}
int sum(int *x, int begin, int end) {
    int sumtemp = 0;
    for (int k = begin; k < end; k++) {
        sumtemp = sumtemp + x[k];
    }
    return sumtemp;
}
int main(int argc, char* argv[]){
    FILE *fp = fopen("Input100k.txt", "r");
    FILE *output = fopen("out.txt", "w+");
    int count = 0;
    int numbers; //buffer for each line
    int fd[2];
    pipe(fd);

    while(fscanf(fp, "%d\n", &numbers) != EOF){
        num[count] = numbers;
        count++;
    }
    fclose(fp);  
    startTime = clock();

    printf("%d\n", max(num, 0, count));

    int partitions = 8;
    int currentPart = 0;
    int levels = 3;
    int section = count/partitions;

    printf("count, section, partition, %d.%d.%d\n", count, section, partitions); //proof of concept

    for (int i = 0; i < levels; ++i)
    {
        if (fork() == 0) //creates a random process tree 
        {
            printf("PID: %d, Parent PID: %d\n", getpid(), getppid());
            minSect = min(x, currentPart*section, (currentPart+1)*section);
            maxSect = max(x, currentPart*section, (currentPart+1)*section);
            sumSect = sum(x, currentPart*section, (currentPart+1)*section);

        }
    }

    fclose(output);
    elapsedTime = (clock() - startTime);
    printf("Total time: %0.3f ticks from Problem 0 part b.(1 tick = 1/1000000s)\n", elapsedTime);
    
    return(0);
}