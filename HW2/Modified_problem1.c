#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()

pthread_t tid[2];
unsigned int shared_data = 0;
pthread_mutex_t mutex;
unsigned int rc;
int arr[10000];/* array*/

int check = 0;

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

    rc = pthread_mutex_lock(&mutex);


    for(size_t x=2; x<i; x++){
    
    	if(check == 0){
        	arg->output[x] = arg->output[x-2] + arg->output[x-1];
        	printf("Generated\n");
        	check = 1;

        }
	    
    }
    rc = pthread_mutex_unlock(&mutex);

}


void* Print(void* ptr){
	thread_args *arg = (thread_args *)ptr;
    size_t i = (size_t)arg->input;

    rc = pthread_mutex_lock(&mutex);


    for(size_t x=2; x<i; x++){
    
    	if(check == 1){
        	arr[x] = arg->output[x];
        	printf("Print: %d\n", arr[x]);
        	check = 0;
        }else{
	        rc = pthread_mutex_unlock(&mutex);
	    }
    }
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf( "Please input a positive integer as the argument.");
        exit(1);
    }
	arr[0]=0;
	arr[1]=1;
    int n = atoi(argv[1]);
    thread_args args;

    if( n <= 0){
        printf( "command line arguent: %s is not a valid positive int\n", argv[1]);
        exit(1);
    }


    pthread_create(&tid[0],0,&Generate,(void*) &args);
    pthread_create(&tid[1],0,&Print,(void*) &args);
    

    sleep(2);

    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    return 0;
}

