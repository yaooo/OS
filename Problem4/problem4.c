#include "problem4.h"
extern int NEGLIM;

int main(int argc, char **argv) {
	char fileName[MAXFILELEN];
	char *input;
	char message[MAXFILELEN];
	int result;
	btNode *root; 
	if (argc > 1) 
		ERROR("Too many arguments. Usage: ./problem4.exe");
	printf("Enter name of input text file: ");
	if (gets(fileName) == NULL) {
		ERROR("Unable to read name of file\n");
	}

	input = fileToStr(fileName);
	if (input == NULL) {
		snprintf(message, MAXFILELEN, "Unable to convert %s to a string\n", fileName);
		ERROR(message);
	}

	printf("Input:\n%s\n", input);
	root = generateTree(input);

	free(input);
	printf("tree generated\n");
	printBT(root, 0);
	// printf("Expected result: %d\n", root->pid);

	// now evaluate expression via processes
	
	// result=procEval(&root);
	
	if (root!=NULL)
		// deleteTree(&root);
	return 0;
}

/*
int procEval(btNode **tree) { 
	// algorithm: given an operand in the tree... 
	// fork and pipe/send the output to a child process to compute the evaluation of the left and right children
	// continue and create more pipes for each operand seen- return by pipes the result, which can be stored along side the operator root
	// traverse the tree using DFS
	
	// inspect node and its children- we need only to know if the node contains an operator or not- this is true if the integer data is < 0
	if (tree!=NULL) {
		btNode *currNode = *tree;
		int currData = currNode->cData;
		int lData=0;
		int rData=0;
		int res, cid;
		// reserve fd0 for parent, fd1, fd2 for children
		int fd0[2];
		int fd1[2];
		int fd2[2];
		char out[MAXDIGITS];
		char in[MAXDIGITS];

		if (pipe(fd0) == -1)
			ERROR("Couldn't create pipe\n");
		if (pipe(fd1)==-1)
			ERROR("Couldn't create pipe\n");
		if (pipe(fd2)==-1)
			ERROR("Couldn't create pipe\n");

		if (currNode->left)
			lData=(currNode->left)->iData;
		if (currNode->right)
			rData=(currNode->right)->iData;
		// case 0: current node is a number
		if(currNode->)
		// case 1: both children are numbers
		if(lData >=0 && iData >=0) {
			// close writing to fd0
			close(fd0[1]);
			// read from fd0 the operator
			read(fd0[0], in, MAXDIGITS);
			// do the correct operation
			res=(in, lData, iData);
			itoa(res, out, MAXDIGITS);
			// return the value to be read via pipe1
			close(fd1[0]);
			write(fd1[1], out, MAXDIGITS);
			close(fd1[1]);
			exit(0);
		}
		// case 2: right child is an operator
		if(lData >= 0 && iData <0) {
			cid=fork();
			if (cid >0) {
				// parent	
				// close reading side
				close(fd0[0]);
				in[0]
				write(fd0[1], in, MAXLEN);
			}
			
		}
	}
	
	
}	
*/
