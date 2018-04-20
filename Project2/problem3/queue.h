# ifndef __MPQUEUE__
#define __MPQUEUE__

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


typedef struct QNODE {
    void* data;
    struct QNODE* next;
} queue_node;

typedef struct queue_t {
    queue_node* rear;
    int size;
} queue_t;

void create_queue(queue_t* q);

//Enqueues data into the queue
void qenqueue(queue_t* q, void* data);


// Dequeues from the queue and passes pointer of dequeued to data
void qdequeue(queue_t* q, void** data);

//Returns 1 if the item exists in the queue
// 0 otherwise
// Requires a function pointer which accesses compares 2 pointers in the queue
// should return 0 if they are equal
// -1 if d1 < d2 (optional)
// 1 if d1 > d2 (optional)
int qexists(queue_t* q, void* data, int (*func)(void* d1, void* d2));


// Deletes an item from the queue if it exists.
// data is NULL if item not found
// Otherwise it is populated with the deleted item
void qdelete_item(queue_t* q, void* to_delete, void** data, int (*func)(void* d1, void* d2));

// Search in a queue for an item
// data = item containing key to compare with
// ret_data, pointer to store return value in
// func - compareTo function
int qsearch(queue_t *q, void *data, void **ret_data, int (*func)(void *d1, void *d2));
// Free everything in the queue
//void qdelete_queue(); Not implemented

#endif