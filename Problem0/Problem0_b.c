#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int num[100000];
int len = 0;
clock_t startTime;
double elapsedTime;

// Helper funtion for printing the array
void print_array(int array[], int len) {    // print array on screen
    int b;
    printf("Array: ");
    for (b = 0; b < len ; b++) {
        printf("%d\t", array[b]);
    }
    printf("\n");
    
}

// min, max, sum functions
int min(int *x, int len) {
    int mintemp = x[0], i;
    for (i = 1;i < len;i++) {
        if(x[i] < mintemp) {
            mintemp = x[i];
        }
    }
    return mintemp;
}
int max(int *y, int len) {
    int maxtemp = y[0], j;
    for (j = 1;j < len;j++) {
        if(y[j] > maxtemp) {
            maxtemp = y[j];
        }
    }
    return maxtemp;
}
int sum(int *z, int len) {
    int sumtemp = 0, k;
    for (k = 0;k < len;k++) {
        sumtemp = sumtemp + z[k];
    }
    return sumtemp;
}
int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Error. Please follow the instruction of executing this program in ReadMe.\n");
        return 1;
    }

    char *inputFileName = argv[1];
    char *OutputFileName = argv[2];

    if(access(inputFileName, F_OK ) != -1){      //check if the file exists
        FILE *fp = fopen(inputFileName,"r");
        // int len = 0;
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
    
    startTime = clock();

    FILE *out1 = fopen(OutputFileName, "w+");
    
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);

    if(fork() == 0){
        int min1 = min(num, len);
        write(fd1[1], &min1, sizeof(min1));
        pid_t pid = fork();
        fprintf(out1, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
        
                if(pid == 0){
                    int max1 = max(num,len);
                    write(fd2[1], &max1, sizeof(max1));
                    exit(0);
                }else{
                    wait(NULL);
                    exit(0);
                }
        
    }else{
        wait(NULL);
        
        int min1, max1;
        read(fd1[0], &min1, sizeof(min1));
        read(fd2[0], &max1, sizeof(max1));

        int sum1 = sum(num, len);
        fprintf(out1, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());

        fprintf(out1,"Max=%d\n", max1);
        fprintf(out1,"Min=%d\n", min1);
        fprintf(out1,"Sum=%d\n", sum1);

    }
    fclose(out1);
    printf("Output file is generated for problem 0 part b.\n");

    elapsedTime = (clock() - startTime);
    printf("Total time: %0.3f ticks from Problem 0 part b.(1 tick = 1/1000000s)\n", elapsedTime);
    

    return 0;
}



