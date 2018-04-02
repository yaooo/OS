#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()

unsigned int shared_data = 0;
unsigned int rc;
int arr[10000];/* array*/

static int check = 0;

typedef struct
{
    int input;
    int output[10000];
} thread_args;

void* Generate(void *ptr)
{
    thread_args *arg = (thread_args *)ptr;
    size_t i = (size_t)arg->input;
    arg->output[0] = 0;
    arg->output[1] = 1;


    for (size_t x = 2; x < i; x++) {
        // Have a spin lock to alternate the numbers.
        while (check == 1);

        if (check == 0) {
            arg->output[x] = arg->output[x - 2] + arg->output[x - 1];
            //printf("Generated\n");
            check = 1;

        }

    }
    pthread_exit(0);
}


void* Print(void* ptr) {
    thread_args *arg = (thread_args *)ptr;
    size_t i = (size_t)arg->input;

    if (i >= 1)
        printf("\t%d\n", arr[0]);
    if (i >= 2)
        printf("\t%d\n", arr[1]);

    for (size_t x = 2; x < i; x++) {

        // Have a spin lock to alternate the numbers.
        while (check == 0);

        if (check == 1) {
            arr[x] = arg->output[x];
            printf("\t%d\n", arr[x]);
            check = 0;
        }

    }
    pthread_exit(0);

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf( "Please input a positive integer as the argument.");
        exit(1);
    }
    arr[0] = 0;
    arr[1] = 1;
    int n = atoi(argv[1]);
    thread_args args;
    args.input = n;

    if ( n <= 0) {
        printf( "command line argument: %s is not a valid positive int\n", argv[1]);
        exit(1);
    }
    pthread_t t0, t1;

    printf("Printing the fabrocini number(s)....\n");
    pthread_create(&t0, NULL, &Generate, (void*) &args);
    pthread_create(&t1, NULL, &Print, (void*) &args);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    return 0;
}
