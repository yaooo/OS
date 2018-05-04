#include <stdio.h> 
#include <stdbool.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

//cinditional variable fro all five philosophers
pthread_cond_t cond[N];

bool chopsticks_available[N]; 
pthread_mutex_t lock;

// if the next dish is avaliable
void test(int index){
    if (!chopsticks_available[index] || !chopsticks_available[(index+1) % N]){  // if not both chopsticks are available
        pthread_cond_wait(&cond[index], &lock);    // wait for them to both be available
        test(index);
    }
}


void mark_chops_unavailable(int i){
    //take the two chopsticks
    chopsticks_available[i] = false;
    chopsticks_available[(i+1) % N] = false;

    pthread_cond_signal(&cond[(i+N-1)%N]);
    pthread_cond_signal(&cond[(i+1)%N]);
}


void *philosopher(void * param){
    int *temp = param;
    int i = *temp;

    printf("Philosopher %d is hungry.\n",i);
    sleep(2);
    
    do {

        pthread_mutex_lock(&lock);

            //wait for chops to be available
            test(i); 

            //if chops are available, take them           
            mark_chops_unavailable(i);

        pthread_mutex_unlock(&lock);

        
        // eating
        printf("Philosopher %d is eating.\n",i);

        sleep(2);

        pthread_mutex_lock(&lock);

            //puts down the chopsticks
            chopsticks_available[i] = true;
            chopsticks_available[(i+1) % N] = true;
            
            pthread_cond_signal(&cond[(i+4)%N]);   // signal to neighbors that he's done
            pthread_cond_signal(&cond[(i+1)%N]);

            //THINKING
            printf("Philosopher %d is thinking.\n",i);
            sleep(2);

            //HUNGRY AGAIN
            printf("Philosopher %d is hungry.\n",i);
            sleep(2);
            pthread_cond_wait(&cond[i], &lock); //wait for neighbors to eat before


        pthread_mutex_unlock(&lock);

    } while(true);
}

int main(){
    pthread_t tid[N];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    for(int i = 0;i<N;i++){
        pthread_cond_init(&cond[i], NULL);
        chopsticks_available[i] = true;
    }

    pthread_mutex_init(&lock, NULL);

    int starting_id[N];

    for(int i = 0;i<N;i++){
        starting_id[i] = i;
        pthread_create(&tid[i], &attr, philosopher, (void *) &starting_id[i]);
    }

    sleep(30);

    for(int i = 0;i<N;i++){
        pthread_cancel(tid[i]);
    }

    return 0;
}
