#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int num[200];
char *name = "Input.txt";   //we can change this so that it copys the file name of the generated file
int len = 0;
clock_t startTime, endTime;
double elapsedTime;

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
    if(access(name, F_OK ) != -1){      //check if the file exists
        FILE *fp = fopen(name,"r");
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
    
    print_array(num, len);
    printf("Output file generated for problem 0 part a.\n");
    
    startTime = clock();
    FILE *out = fopen("Output_problem0_part_a.txt", "w+");
    fprintf(out, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
    fprintf(out,"Max=%d\n", max(num, len)); // pass array and len
    fprintf(out,"Min=%d\n", min(num, len));
    fprintf(out,"Sum=%d\n", sum(num, len));
    
    fclose(out);
    endTime = clock();
    elapsedTime = ((endTime - startTime)/CLOCKS_PER_SEC);
    printf("%f\n", elapsedTime);
    
    
    
    FILE *out1 = fopen("Output_problem0_part_b.txt", "w+");
    
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
                }else{
                    wait(NULL);
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

    return 0;
}



