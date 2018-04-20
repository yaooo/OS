#include "queue.h"

void create_queue(queue_t *q)
{
    q->size = 0;
    q->rear = NULL;
    return;
}

void qenqueue(queue_t *q, void *data)
{
    queue_node *nn = malloc(sizeof(queue_node));
    nn->data = data;
    if (q->rear == NULL)
    {
        q->rear = nn;
        nn->next = nn;
        q->size++;
    }
    else
    {
        nn->next = q->rear->next;
        q->rear->next = nn;
        q->rear = nn;
        q->size++;
    }
}

void qdequeue(queue_t *q, void **data)
{
    if (q->rear == NULL)
    {
        // printf("Can't dequeue from null queue\n'");
        return;
    }
    else if (q->rear == q->rear->next)
    {
        *data = q->rear->data;
        free(q->rear);
        q->rear = NULL;
        q->size--;
        return;
    }
    else
    {
        // printf("freeing Q rear next %p\n", q->rear->next);
        // printf("q-> rear %p\n", q->rear);
        // printf("q->rear next: %p\n", q->rear->next);
        queue_node* old = q->rear->next;
        *data = old->data;
        q->rear->next = old->next;
        free(old);
        q->size--;
        return;
    }
}

int qexists(queue_t *q, void *data, int (*func)(void *d1, void *d2))
{
    if(q->rear == NULL) {
        return 0;
    }
    queue_node *curr = q->rear->next;
    do
    {
        if (func(curr->data, data) == 0)
        {
            return 1;
        }
    } while ((curr = curr->next) != q->rear->next);
    return 0;
}

int qsearch(queue_t *q, void *data, void **ret_data, int (*func)(void *d1, void *d2))
{
    queue_node *curr = q->rear->next;
    do
    {
        if (func(curr->data, data) == 0)
        {
            *ret_data = curr->data;
            return 1;
        }
    } while ((curr = curr->next) != q->rear->next);
    *ret_data = NULL;
    return 0;
}

void qdelete_item(queue_t *q, void *to_delete, void **data, int (*func)(void *d1, void *d2))
{
    if (q->rear == NULL)
    {
        return;
    }
    queue_node *prev = q->rear;
    queue_node *curr = q->rear->next;
    do
    {
        if (func(curr->data, to_delete) == 0)
        {
            if (curr->next == curr)
            {
                q->rear = NULL;
                *data = curr->data;
                free(curr);
                q->size--;
                return;
            }
            else
            {
                prev->next = curr->next;
                *data = curr->data;
                free(curr);
                q->size--;
                return;
            }
        }
        prev = curr;
    } while ((curr = curr->next) != q->rear->next);
}