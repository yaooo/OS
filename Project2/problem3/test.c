#include "queue.h"
#include <assert.h>

int ptr_cmp(void* d1, void* d2) {
    if(d1 == d2) {
        return 0;
    }
    return -1;
}
int int_cmp(void* d1, void* d2) {
    if( *(int*)d1 == *(int*)d2) {
        return 0;
    }
    return -1;
}

int main() {
    
    queue_t q;
    
    create_queue(&q);
    assert(q.rear == NULL);
    int i1 = 50;
    qenqueue(&q, &i1);
    assert(q.size == 1);
    int i2 = 10;
    qenqueue(&q, &i2);
    assert(q.size == 2);
    assert(q.rear->next->next->data == &i2);
    assert(qexists(&q, &i2, &ptr_cmp) == 1);
    assert(qexists(&q, NULL, &ptr_cmp) == 0);
    int* i3;
    qdequeue(&q, (void**)&i3);
    assert(i3 == &i1);
    assert(q.size == 1);

    
    qdequeue(&q, (void**)&i3);
    assert(i3 == &i2);
    assert(q.size == 0);

    int is[5] = {1, 2, 3, 4, 5};
    qenqueue(&q, &(is[0]));
    qenqueue(&q, &(is[1]));
    qenqueue(&q, &(is[2]));
    qenqueue(&q, &(is[3]));
    qenqueue(&q, &(is[4]));
    assert(q.size == 5);
    int s = 3;
    int* plc;
    qdelete_item(&q, &s, (void**)&plc, &int_cmp);
    assert(q.size == 4);
    assert(qexists(&q, &(is[0]), &ptr_cmp) == 1);
    assert(qexists(&q, &(is[1]), &ptr_cmp) == 1);
    assert(qexists(&q, &(is[3]), &ptr_cmp) == 1);
    assert(qexists(&q, &(is[4]), &ptr_cmp) == 1);

    printf("All tests passed\n");
    exit(0);

}