#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <math.h>

static int arr[10000];/* array*/
static int index = 0; // length of the output array with all prime numbers
typedef struct{
        int input;
        int output[10000];
} thread_args;

void *thread_func ( void *ptr )/*child thread */{
    thread_args *arg = (thread_args *)ptr;
    size_t i = (size_t)arg->input;
    
    for(size_t x=0; x<i; x++){
        arg->output[x] = arr[x]; //0 means true
    }

    for(size_t x=2; x< sqrt((double) i); x++){
        if(arg->output[x] == 0){
            for(size_t j = x*x; j <= i; j += x){
                arg->output[j] = 1;
            }
        }
    }
    printf("Prime number(s):\n");

    for(size_t x=2; x<i; x++){
        if(arg->output[x] == 0){
            printf("%d\n", (int) x);
            arr[index] = (int)x;
            index ++;
        }
    }

    pthread_exit(0);
} 

int reversDigits(int num)
{
    int rev_num = 0;
    while (num > 0)
    {
        rev_num = rev_num*10 + num%10;
        num = num/10;
    }
    return rev_num;
}

void *traverse( void *ptr ){
    for(int i = 0; i < index; i++){
        for(int j = i+1; j < index; j++){
            if(reversDigits(arr[j]) == arr[i]){
                printf("Reversed pair: %d\t%d\n", arr[i], arr[j]);
            }
        }
    }
    pthread_exit(0);
} 

int main(int argc, char *argv[]){
    if (argc < 2){
        printf( "Please input a positive integer as the argument.");
        exit(1);
    }
    pthread_t thread, thread1;
    thread_args args;
    int status;
    int i;

    int n = atoi(argv[1]);

    if( n <= 0){
        printf( "command line arguent: %s is not a valid positive int\n", argv[1]);
        exit(1);
    }

    args.input = n;

    pthread_create(&thread,NULL,thread_func,(void*) &args );
    pthread_join(thread, NULL);

    pthread_create(&thread1,NULL,traverse,0);
    pthread_join(thread1, NULL);

    return 0;
}