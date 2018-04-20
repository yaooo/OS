#include "mypthread.h"

#define NUM_THREADS 10

void *print(void *arg)
{
    printf("THREAD FUNCTION\n");
    mypthread_exit(0);
}

void mypthread_test()
{

    mypthread_t tid;
    printf("Creating thread\n");
    mypthread_create(&tid, NULL, print, NULL);
    printf("Joining on thread\n");
    mypthread_join(tid, NULL);
    return;
}

void mypthread_test2(){
    mypthread_t tid[2];

    printf("Creating thread\n");
    mypthread_create(&tid[0], NULL, print, NULL);
    mypthread_create(&tid[1], NULL, print, NULL);
    mypthread_join(tid[0], NULL);
    mypthread_join(tid[1], NULL);

}

int main()
{
    mypthread_test();
    mypthread_test2();
    // mypthread_exit(0);
    return 0;
}