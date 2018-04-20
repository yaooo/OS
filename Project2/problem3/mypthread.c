#include "mypthread.h"

static int threadCount = 1;
static int num_Threads = 0;
static int excutingThread = 1;

struct threadNode {
	struct threadNode * next; // next thread
	mypthread_t *ptr; // pointer to thread
};

struct threadNode* head; //head node
struct threadNode* tail; //tail node
struct threadNode* temp; //temp node

struct threadNode* create_node() {
	struct threadNode* tmp;
	mypthread_t threadname;
	// initialization
	tmp = (struct threadNode *) malloc(sizeof(struct threadNode)); 
	tmp->next = NULL;
	return tmp;
}

//	create a new thread and add it to the queue
void add_to_list(mypthread_t* threadname) {
	struct threadNode* t = create_node();
	num_Threads++;
	if (head) {
		tail->next = t;
		tail = t;
		tail->next = head;
	}
	else {
		head = t;
		tail = t;
	}
	t->ptr = threadname;
}

mypthread_t* find_thread(int id) {
	struct threadNode* tmp = head;
	// check the circular linked list
	if (tmp->ptr->tid == id)
		return (tmp->ptr);
	else
		tmp = tmp -> next;

	while (tmp != head) {
		if (tmp->ptr->tid == id)
			return (tmp->ptr);//find it
		tmp = tmp->next;
	}
	printf("Thread not found\n", id);
	return NULL;
}

// find the next active thread
mypthread_t* find_next_active(int currentid) {
	struct threadNode* tmp = head; //temp node
	// run through the circular linked list and find the next active thread
	do {
		if (tmp->ptr->tid == currentid)
		{
			tmp = tmp->next;
			while (1) {
				// if active
				if (tmp->ptr->state == 0) {
					return (tmp->ptr);//find it
				}
				tmp = tmp->next;
			}
			break;
		}
		tmp = tmp->next;
	} while (tmp != head);
	return NULL;
}

// Create the first thread(parent thread)
mypthread_t* createParentThread() {
	mypthread_t* temp = (mypthread_t *)malloc(sizeof(mypthread_t));
	//set tid
	temp->tid = threadCount++;
	//set up the context of parent thread
	ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
	temp->ucp = context;
	//allocate space and stack pointer
	temp->ucp->uc_stack.ss_sp = malloc(1024 * 4);
	temp->ucp->uc_stack.ss_size = 1024 * 4;
	//set to active state
	temp->state = 0;
	return temp;
}

// Allocate space and stack pointer
void allocation(mypthread_t *thread){
	thread->ucp->uc_stack.ss_sp = malloc(1024 * 4);
	thread->ucp->uc_stack.ss_size = 1024 * 4;
	thread->tid = threadCount++;
	//set state to active
	thread->state = 0;
}

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
	if (num_Threads == 0) // first thread being created
	{
		//create the parent thread
		mypthread_t* first_thread_created = createParentThread();
		add_to_list(first_thread_created);
	}
	//set up context of thread
	ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
	thread->ucp = context;
	getcontext(thread->ucp);
	
	//allocate space and stack pointer
	allocation(thread);
	
	//make context with given function
	makecontext(thread->ucp, (void(*)()) start_routine, 1, arg);
	
	//add thread to the list
	add_to_list(thread);
	return 0;
}

void mypthread_exit (void *retval) {
	//find the thread
	mypthread_t* current = find_thread(excutingThread);
	//set thread state to dead
	current->state = 2; 
	free(current->ucp);
	if (current->jointid != 0) {
		mypthread_t* joining_thread = find_thread(current->jointid);
		//set state to active
		joining_thread->state = 0;
	}
	//find next active thread
	mypthread_t* nextThread = find_next_active(current->tid);

	//Make sure do not quit the next thread if it happens to be the same one
	//Do context switching
	if (excutingThread != nextThread->tid) {
		excutingThread = nextThread->tid;
		setcontext(nextThread->ucp);
	}
}

int mypthread_yield (void) {
	mypthread_t* current = find_thread(excutingThread);
	//find the next thread in the list
	mypthread_t* nextThread = find_next_active(current->tid);

	//if the next active is a different thread, then do context switching
	if (excutingThread != nextThread->tid) {
		//set the tid of the excuting thread
		excutingThread = nextThread->tid;
		swapcontext(current->ucp, nextThread->ucp);
	}
	return 0;
}


int mypthread_join (mypthread_t thread, void **retval) {
	short id = thread.tid;
	// fidn the joining thread
	mypthread_t* t_thread = find_thread((int)id);

	//find the executing thread
	mypthread_t* current = find_thread(excutingThread);

	//if thread active, set the executing thread state to blocked
	if (t_thread->state == 0) {
		current->state = 1;

		t_thread->jointid = excutingThread;
		excutingThread = id;
		//context switching
		swapcontext(current->ucp, t_thread->ucp);
	}
	return 0;
}