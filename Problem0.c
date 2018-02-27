#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int num[200];
char *name = "Input.txt";
int len = 0;


void print_array(int array[], int len) {
    // print array on screen
    int b;
    printf("Array: ");
    for (b = 0; b < len ; b++) {
        printf("%d\t", array[b]);
    }
    printf("\n");

}

// function
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
    //if the file exists
    if(access(name, F_OK ) != -1){
        FILE *fp = fopen(name,"r");
        // int len = 0;
        while(1) {

            int goodData = fscanf(fp, "%d", &num[len]);
            len++;
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
    
    FILE *out = fopen("Output_problem0_part_a.txt", "w+");
    fprintf(out, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
  
    fprintf(out,"Max=%d\n", max(num, len)); // pass array and len
    fprintf(out,"Min=%d\n", min(num, len));
    fprintf(out,"Sum=%d\n", sum(num, len));

    fclose(out);




    int max1, min1, sum1;
    int count = 0;
    FILE *out1 = fopen("Output_problem0_part_b.txt", "w+");
    fprintf(out1, "hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
  

    max1 = max(num, len);
    
    if(fork() == 0){
        min1 = min(num, len);
    
    }
    
    if(fork() == 0){
        sum1 = sum(num, len);
        count= 3;
    }
    

    fprintf(out1,"hi, i\'m process %d and my parent is %d\n", getpid(), getppid());
    if(count == 3){
        fprintf(out1,"Max=%d\n", max1); // pass array and len
        fprintf(out1,"Min=%d\n", min1);
        fprintf(out1,"Sum=%d\n", sum1);
    }
    fclose(out1);













    return 0;
}



