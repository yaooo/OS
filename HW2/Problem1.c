#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()

int arr[10000];/* array*/

typedef struct
{
    int input;
    int output[10000];
} thread_args;


void *thread_func ( void *ptr )/*child thread */
{
    thread_args *arg = (thread_args *)ptr;
    size_t i = (size_t)arg->input;
    arg->output[0] = 0;
    arg->output[1] = 1;

    for ( size_t x = 2; x < i; x++ )
    {
        arg->output[x] = arg->output[x - 2] + arg->output[x - 1];
    }
    pthread_exit(0);
} // end function: thread_func


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf( "Please input a positive integer as the argument.");
        exit(1);
    }

    pthread_t thread;
    thread_args args;
    int status;
    int i;

    int n = atoi(argv[1]);

    if ( n <= 0) {
        printf( "command line argument: %s is not a valid positive int\n", argv[1]);
        exit(1);
    }

    args.input = n;

    status = pthread_create(&thread, NULL, thread_func, (void*) &args );
    if ( status )
    {   // then error occurred
        perror( "phread_create failed to create thread" );
        exit(1);
    }

    pthread_join(thread, NULL);
    printf("Fibonacci number:\n");/*print all numbers*/
    for (i = 0; i < n; i++)
    {
        arr[i] = args.output[i]; /* get the result*/
        printf("%d\t", arr[i]);/*print all numbers*/
    }
    printf("\n");
    return 0;
}