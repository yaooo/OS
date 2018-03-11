#include "problem4.h"
int NEGLIM = -10000;
/* stack related and BT related functions */
/* stack related functions */
int push(stackNode **s, int c, int dataType) {

	stackNode *curr;
	curr = malloc(sizeof(stackNode));
	if (curr == NULL) {
		printf("Unable to malloc data\n");
		return 1;
	}
	/* depending on the data type fill in the corresponding data */
	if (dataType == 0) {
		curr->cData = c;
		curr->iData=NEGLIM;
	}
	if (dataType==1) {
		curr->iData=c;
		curr->cData=-1;
		// for characters, the negative limit is -16
	}
	curr->next = NULL;
	/* add/link this newly created node to linked list such that it's on top */
	curr->next = *s;
	*s = curr; 
	return 0;
}

int pop(stackNode **s) {
	int c;
	// printf("*s->iData = %d\n", (*s)->iData);
	// printf("*s->cData = %d\n", (*s)->cData);
	if (ifEmpty(s)) { 
		ERROR("Can't pop from empty stack\n");
	}
	stackNode *temp = *s;
	// note either iData OR cData = NEGLIM, so this is ok
	c = temp->iData;
	if (c== NEGLIM)
		c = temp->cData;
	(*s) = (*s)->next;
	free(temp);
	temp=NULL;
	return c;
}

int ifEmpty(stackNode **s) {
	if (*s == NULL)
		return 1;
	return 0;
}

int peek(stackNode *s) {
	int c;

	if (ifEmpty(&s)) {
		// printf("Can't peek from empty stack\n");
		ERROR("Can't peek from empty stack\n");
		return NEGLIM;
	}
	c = s->iData;
	if (c == NEGLIM) 
		c= s->cData;

	return c;
}

btNode *addNode(int val, int type) {
	btNode *newNode = malloc(sizeof(btNode));
	// printf("called to add node\n");
	if (newNode==NULL)
		ERROR("Couldn't malloc data for a new node\n");
	if (type==0) {
		newNode->cData=val;
		// printf("%c added\n", newNode->cData);
		newNode->iData=NEGLIM;
	}
	else {
		newNode->iData=val;
		// printf("%d added\n", newNode->iData);
		newNode->cData=-1;
	}
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent=NULL;
	return newNode;
}

void addLeaf(btNode **tree, int val, int type, int pos) {
	btNode *newNode = addNode(val, type);
	btNode *temp = newNode;
	if (pos==0) {
		(*tree)->left = newNode;
	}
	else {
		(*tree)->right = newNode;
	}
}

void addParent(btNode **target, btNode **leaf, int pos) {
	if (pos==0) {
		(*target)->left = *leaf;
	}
	else {
		(*target)->right = *leaf;
	}
}

void printBT(btNode *tree, int shift) {
	if (tree!=NULL) {
		if (tree->right)
			printBT(tree->right, shift+SHIFT);
		int i=0;
		for (i;i<shift;i++)
			printf(" ");
		if (tree->cData <=0 ) {
			if (tree->iData != NEGLIM)
				printf("%d", tree->iData);
			printf("\n", tree->iData);
		}

		else {
			if (tree->iData != NEGLIM)
				printf("%c", tree->cData);
			if (tree->iData!=NEGLIM) 
				printf(", %d", tree->iData);
			printf("\n");

		}

		if (tree->left)
			printBT(tree->left, shift + SHIFT);
	}
}

// pseudo delete function
void deleteTree(btNode **tree) {
	btNode *lChild;
	btNode *rChild;
	if (*tree!=NULL) {
		lChild = (*tree)->left;
		rChild = (*tree)->right;
		deleteTree(&lChild);
		deleteTree(&rChild);
		free(*tree);
		*tree=NULL;
		/*
		(*tree)->cData=-1;
		(*tree)->iData=NEGLIM;
		(*tree)->left=NULL;
		(*tree)->right=NULL;
		*tree=NULL;
		*/
	}
}

btNode *mergeSubTrees(btNode **subTreeArr, int numSubTrees, int placeHolder) {
	int finish=0;
	btNode *head, *temp1, *temp2;
	int counter=-1;
	
	for (counter;counter>placeHolder;counter--) {
		int i=0;
		int iData;
		char cData;
		temp1=NULL;
		temp2=NULL;
		// attempt to find the subtree that has an operator and has the specific placeholder to be found, as indicated by counter
		// find subtree with a child equivalent to the placeholder
		for (i;i<numSubTrees;i++) {
			locate(subTreeArr[i], counter, &temp1);
			if (temp1!=NULL) {
				printf("found something whose operator has the placeholder = %d\n", counter);
				printBT(temp1, 0);
			}
			
			// now try to find the node that contains only the placeholder
			// printf("locate in this subtree\n");
			// printBT(subTreeArr[i], 0);
			locateParent(subTreeArr[i], counter, &temp2);
			if (temp2!=NULL) {
				printf("child containing placeholder = %d\n", counter);
				printBT(temp2, 0);	
			}

			if (temp2!=NULL && temp1!=NULL)
				break;
		}
		
		// now link temp1, temp2 if possible
		if (temp1 == NULL || temp2 == NULL)
			ERROR("Couldn't merge trees\n");

		btNode *lChild, *rChild;
		lChild = temp2->left;
		rChild = temp2->right;
		if (lChild!=NULL && rChild != NULL) {
			// case where the placeholder was repeated- get rid of repetition
			// printf("lChild->iData. lChild->cData; rChild->iData, rChild->cData: %d, %d; %d, %d\n", lChild->iData, lChild->cData, rChild->iData, rChild->cData);
			if(lChild->iData == counter && rChild->iData == counter) {
				// printf("deleting a child\n");

				if (lChild->cData==-1) {
					// printf("deleting left\n");
					deleteTree(&lChild);
				}
				else {
					// printf("deleting right\n");
					deleteTree(&rChild);
				}

		}
			// otherwise, the leftChild might have the data of the place holder
			else if(lChild->iData==counter) {
				deleteTree(&lChild);
				temp2->left = temp1;
		
			}
			else {
				deleteTree(&rChild);
				temp2->right = temp1;
			}

		}
		// case where rChild is NULL, lChild must contain the placeholder
		else if(lChild!=NULL) { 
			deleteTree(&lChild);
			temp2->left = temp1;
		}
		else {
			deleteTree(&rChild);
			temp2->right = temp1;
		}
		head = temp2;
		printf("merged subtree\n");
		printBT(head, 0);

	}
	// printf("done\n");

	return head;
}

void locate(btNode *tree, int placeHolder, btNode **target) {
	if(tree != NULL && *target==NULL) {
		if (tree->cData != -1 && tree->iData == placeHolder) {
			*target = tree;
			return;
		}
		if (tree->left)
			locate(tree->left, placeHolder, target);
		if (tree->right)
			locate(tree->right, placeHolder, target);
	}
}

void locateParent(btNode *tree, int placeHolder, btNode **target) {
	
	btNode *lChild, *rChild;
	lChild = tree->left;
	rChild = tree->right;
	if(tree != NULL && *target== NULL) {
		// printf("cData, iData, placeHolder: %d, %d, %d\n", tree->cData, tree->iData, placeHolder);
		// printBT(tree, 0);
		
		if (lChild) {
			if (lChild->cData == -1 && lChild->iData==placeHolder) {
				*target = tree;
				return;
			}
			locateParent(lChild, placeHolder, target);
		}
		
		if (rChild) {
			if (rChild->cData == -1 && rChild->iData==placeHolder) {
				*target = tree;
				return;
			}
			locateParent((tree->right), placeHolder, target);
		
		// printf("tree->iData %d\n", tree->iData);
		}
	}
}
