#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 

long long arr[10000];
typedef struct
{
    long long input;
    long long output[10000];
} thread_args;

/*child thread */
// Thread function used to generate the number
void *thread_func ( void *ptr )
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
}


int main(int argc, char *argv[]) {
    
    // Read the input
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
    if ( status ){
        perror( "phread_create failed to create thread" );
        exit(1);
    }

    pthread_join(thread, NULL);
    

    /*parent thread(main thread): print the numbers generated */

    printf("Fibonacci number(s):\n");/*print all numbers*/
    for (i = 0; i < n; i++){
        arr[i] = args.output[i]; /* get the result*/
        printf("  %lld\n", arr[i]);/*print all numbers*/
    }
    return 0;
}