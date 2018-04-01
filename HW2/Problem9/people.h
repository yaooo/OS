#include <stdio.h> // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <pthread.h> // pthread_create(), pthread_exit(), pthread_join()
#include <math.h>

void teacher_enter();
void teach();
void teacher_exit();

void child_enter();
void learn();
void child_exit();

void parent_enter();
void verify_compliance();
void parent_exit();

void go_home();

