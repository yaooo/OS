#include "problem4.h"
extern int NEGLIM;
/* other relevant functions */

char *fileToStr(char *fileName) {
	FILE *fp = fopen(fileName, "r");
	int count=0;
	char c;
	int currSize = MAXFILELEN;
	char *buf = malloc(sizeof(char) * currSize);

	if (fp == NULL) {
		printf("Couldn't open file\n");
		return NULL;
	}

	while ((c = getc(fp))!=EOF) {
		buf[count] = c;
		count++;
		if (count >= currSize) {
			buf = safeRealloc(buf, &currSize, sizeof(char));
			if (buf == NULL) {
				printf("Couldn't reallocate memory for a large enough buffer\n");
				return NULL;
			}
		}
	}
	buf[count] = '\0';
	return buf;
}

btNode *generateTree(char *exp) {
	/* use stacks to help generate process tree */
	int counter, i, type, len, numSubTrees;
	stackNode *val = NULL; 
	stackNode *op = NULL; 
	/* newExp = tokenized expression */
	/* token = either an operator or and operand */
	char delim, *newExp, **tokens;
	char strDelim[] = "?";
	btNode *tree, *temp, **subTreeArr; 
	// let negative numbers indicate where some operation should have occurred
	int placeHolder=-1;

	delim = '?';
	counter=0;
	i=0;
	numSubTrees=0;

	newExp = insertDelim(exp, delim);
	len = strlen(newExp);
	if (newExp == NULL) 
		ERROR("Couldn't tokenize expression\n");

	/* we'll have at most the number of tokens == twice the number of entries in the string */
	tokens=malloc(sizeof(char *) * len);

	// we'll have at most len number of subTrees to consider
	subTreeArr = malloc(sizeof(btNode *) * len);
	if (subTreeArr==NULL)
		ERROR("Couldn't malloc memory for subtree\n");
	for (i=0;i<len;i++) {
		tokens[i]=malloc(sizeof(char)*MAXDIGITS);
		if (tokens[i] == NULL)
			ERROR("Couldn't malloc data for tokens\n");
		subTreeArr[i] = malloc(sizeof(btNode)); 
		if (subTreeArr[i] == NULL)
			ERROR("Couldn't malloc data for sub tree\n");
	}
	

	// printf("newExp: %s\n", newExp);
	/* just fill stacks first - this will allow us to determine the binary tree */
	counter=0;
	// printf("newExp has len = %d\n", len);
	tokens[counter]=strtok(newExp, strDelim);
	while ( tokens[counter]!= NULL && counter < len) {
		// now determine token type, place in corresponding stack
		type = detType(tokens[counter]);
		// printf("tokens[%d] to be inserted: %s has type %d\n", counter, tokens[counter], type);
		// printf("current tops for op, val: %c, %d\n", peek(op), peek(val));
		if (type == 0) {
			// the actual operator is just the first character of the token
			char c=tokens[counter][0];
			// printf("c = %c\n", c);
			int check;
			if (c=='(') {
				check = push(&op, c, type);
				if (check)
					ERROR("Could not push onto operator stack\n");
				// printf("pushed %c\n", c);
			}
			else if (c==')') {
				int loopCnt=0;
				while (peek(op) != '(') {
					// printf("evaluating within parenthesis\n");
					char tempOp = pop(&op);
					int val1 = pop(&val);
					int val2 = pop(&val);
					int res;
					char opStr[2];
					opStr[0]=tempOp;
					opStr[1]='\0';
					subTreeArr[numSubTrees] = addNode(tempOp, 0);
					addLeaf(&subTreeArr[numSubTrees], val1, 1, 0);
					addLeaf(&subTreeArr[numSubTrees], val2, 1, 1);
					// res=evaluate(opStr, val1, val2);

					push(&val, placeHolder, 1);
					(subTreeArr[numSubTrees])->iData = placeHolder;
					// (subTreeArr[numSubTrees])->pid = res;
					
					// printf("op %c and %d simplify to placeholder %d\n", tempOp, val1, placeHolder);
					placeHolder--;
					numSubTrees++;
				}
				// printf("subTree added\n");
				// printBT(subTreeArr[numSubTrees], 0);
				// finally pop the '('
				pop(&op);
				// numSubTrees++;
			}
			// case of a normal operator
			else {
				int loopCnt=0;
				while(!ifEmpty(&op) && gtePrec(peek(op), c) && peek(op)!= '(') {
                                        char tempOp = pop(&op);
					char opExp[2];
                                        int val1 = pop(&val);
					// printf("%c is of higher or equal precedence than %c, so we will do this op\n", tempOp, c);
					int val2 = pop(&val);
					opExp[0] = tempOp;
					opExp[1] = 0;
					int res;
                                        subTreeArr[numSubTrees] = addNode(tempOp, 0);
                                        addLeaf(&subTreeArr[numSubTrees], val1, 1, 0);
                                        addLeaf(&subTreeArr[numSubTrees], val2, 1, 1);
					// res=evaluate(opExp, val1, val2);
					push(&val, placeHolder, 1);
					// printf("op %c and %d simplify to placeholder %d\n", tempOp, val1, placeHolder);
					(subTreeArr[numSubTrees])->iData = placeHolder;
					// (subTreeArr[numSubTrees])->pid = res;
					numSubTrees++;
					placeHolder--;
                                }
				/*
				if (loopCnt) { 
                                	// printf("subTree added\n");
                                	// printBT(subTreeArr[numSubTrees], 0);
					numSubTrees++;
				}
				*/
				// printf("pushed %c\n", c);
				check = push(&op, c, type);
				if (check==1)
					ERROR("Could not push onto operator stack\n");
			}
		}
		else if (type == 1) {
			// actual value is just atoi of the token
			int c = atoi(tokens[counter]);
			int check = push(&val, c, type);
			if (check)
				ERROR("Could not push onto value stack\n");
			// printf("pushed %d\n", val->iData);

		}
		counter++;
		tokens[counter] = strtok(NULL, strDelim);
		// printf("counter = %d, placeholder = %d\n", counter, placeHolder);
	}
	// printf("finished with part of stuff\n");

	/* complete the rest of the BT trees */
	
	while (!ifEmpty(&op)) {
			int loopCnt=0;
			while(!ifEmpty(&op)) {
                        	char tempOp = pop(&op);
                                int val1 = pop(&val);
				int val2 = pop(&val);
                        	char opStr[2];
				int res;
				opStr[0] = tempOp;
				opStr[1] = 0;
                                subTreeArr[numSubTrees] = addNode(tempOp, 0);
                                addLeaf(&subTreeArr[numSubTrees], val1, 1, 0);
                                addLeaf(&subTreeArr[numSubTrees], val2, 1, 1);
                                loopCnt++;


				// res = evaluate(opStr, val1, val2);

				// (subTreeArr[numSubTrees])->pid = res;
				push(&val, placeHolder, 1);
				(subTreeArr[numSubTrees])->iData = placeHolder;
				// printf("op %c and %d simplify to placeholder %d\n", tempOp, val1, placeHolder);
				placeHolder--;
				numSubTrees++;
                         }
			/*
			if (loopCnt) { 
                               	// printf("subTree added\n");
                               	// printBT(subTreeArr[numSubTrees], 0);
				numSubTrees++;
			}
			*/
	}

	printf("done -attempt to print out all BTs\n");
	i=0;
	for (i;i<numSubTrees;i++) {
		printBT(subTreeArr[i], 0);
	}
	placeHolder++;
	// now merge all these subTrees
	tree=mergeSubTrees(subTreeArr, numSubTrees, placeHolder);
	// printf("free stuff\n");
	free(newExp);
	free(tokens);
	return tree;

}

int evaluate(char *op, int val1, int val2) {
	if (!strcmp(op,"+"))
		return val1+val2;
	else if (!strcmp(op,"*"))
		return val1*val2;
	else if (!strcmp(op, "-") || !strcmp(op, "/"))
		ERROR("- and / are not required to be supported. Please provide a file that does not contain these specific operations\n");
	else if (!strcmp(op, "(") || !strcmp(op, ")"))
		ERROR("Parentheses should have been evaluated/gotten rid of. There was an error in generating the process tree\n");
	else
		ERROR("Unknown operation detected\n");
}
