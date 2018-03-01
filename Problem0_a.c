#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int num[1000000];
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
int
 main(int argc, char *argv[]){
    char *ip1 = argv[1]; //input = 10 numbers
    char *ip2 = argv[2]; //input = 100 numbers
    char *ip3 = argv[3]; //input = 1000 numbers
    char *ip4 = argv[4]; //input = 10000 numbers
    char *ip5 = argv[5]; //input = 100000 numbers
    if(access(ip1, F_OK ) != -1){      //check if the file exists
        FILE *fp = fopen(ip1,"r");
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
    
    printf("Output file generated for problem 0 part a.\n");
    
    startTime = clock();
    FILE *out = fopen("Output_problem0_part_a.txt", "w+");
    fprintf(out, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
    fprintf(out,"Max=%d\n", max(num, len)); // pass array and len
    fprintf(out,"Min=%d\n", min(num, len));
    fprintf(out,"Sum=%d\n", sum(num, len));
    
    fclose(out);
    elapsedTime = (clock() - startTime)/1000;
    printf("Total time: %0.3fms from Problem 0 part a.\n", elapsedTime);
    

    return 0;
}



