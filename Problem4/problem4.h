#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#define MAXFILELEN 10000
#define SHIFT 6
#define MAXDIGITS 20

extern int NEGLIM;

/* function to read the input file into a string (remove whitespaces) */
char *fileToStr(char *fileName);

/* Functions for debugging/error handling */
/* Reallocate memory-update current size */
void *safeRealloc(void *ptr, int *currSize, int dataSize);

/* prints an error message and exits process */
void ERROR(char *error);

/* function handler for signals */
void handler(int sig);


/* token related functions */
/* insert delimiters into expression and return the modified result*/
char *insertDelim(char *exp, char delim);


/* determines whether the input is an operand or operator */
int detType(char *token);

/* determines if the precdence of the left operator is greater than or equal to that of the right */
int gtePrec(int left, int right);

/* stack data structure definition and related functions*/
/* one node of the stack */
typedef struct Stack {
	char cData;
	int iData;
	struct Stack *next;
} stackNode;

/* expression to do math on two stacks- the left = operators, right = operands */
int doMath(stackNode **op, stackNode **val);

/* push */
int push(stackNode **s, int c, int dataType);

/* pop */
int pop(stackNode **s);

/* check if empty */
int ifEmpty(stackNode **s);

/* peek */
int peek(stackNode *s);


/* Binary tree data structure definition and related functions */
/* some node of the binary tree */
typedef struct bt {
	char cData;
	int iData;
	int pid;
	struct bt *left;
	struct bt *right;
	struct bt *parent;
} btNode;

/* create a new btNode - return it */
btNode *addNode(int val, int type);

/* add left/right leaves to a specific node in the BT */
void addLeaf(btNode **node, int val, int type, int position);

/* add a parent to a specific node in the BT and specify which child of the parent it is */
void addParent(btNode **target, btNode **leaf, int pos);

/* print BT */
void printBT(btNode *tree, int shift);

/* function to generate binary tree to be evaluated given an expression */
btNode *generateTree(char *exp);

/* function to delete binary tree */
void deleteTree(btNode **tree);

/* function to merge binary trees using placeholder rule */
btNode *mergeSubTrees(btNode **subTreeArr, int numSubTrees, int placeHolder);

/* find a node that contains placeHolder only */
void locateParent(btNode *tree, int placeHolder, btNode **target);

/* find a node that contains a placeholdern and operator */
void locate(btNode *tree, int placeHolder, btNode **target);

/* function to evaluate expression */
int procEval(btNode **tree);

/* function to evaluate an expression given an operator and two operands */
int evaluate(char *op, int val1, int val2);
