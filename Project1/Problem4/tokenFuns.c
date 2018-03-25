#include "problem4.h"
extern int NEGLIM;
/* token related functions + reading/interpreting data */
/* insert delimiter */
char *insertDelim(char *exp, char delim) {
	char *newExp=malloc(sizeof(char)*MAXFILELEN);
	int numNumbers=0;
	int numOperators=0;
	int other=0;
	int counter=0;
	int subCounter=0;
	int currLen = MAXFILELEN;
	while(exp[counter]!='\0') {
		// insert a delimiter between every 'number' and operator
		if (exp[counter] >= '0' && exp[counter] <= '9') { 		
			numNumbers++;
			if (numOperators>0 || other>0) {
				if (other)
					other=0;
				else
					numOperators=0;
				newExp[subCounter] = delim;
				subCounter++;
				if (subCounter>=currLen) { 
					newExp=safeRealloc(newExp, &currLen, sizeof(char));
					if (newExp == NULL) {
						printf("Reallocation error\n");
						return NULL;
					}
				}
							

			}
			newExp[subCounter] = exp[counter];
			subCounter++;
		}
		else if(exp[counter] == '+' || exp[counter] == '-' || exp[counter] == '*' || exp[counter] == '/' || exp[counter] == '(' || exp[counter] == ')'  ) {
			if (numNumbers > 0 || other > 0) {
				if (numNumbers > 0)
					numNumbers=0;	
				else
					other=0;
				newExp[subCounter]=delim;
				subCounter++;
				if (subCounter >= currLen) {
					newExp = safeRealloc(newExp, &currLen,sizeof(char));
					if (newExp == NULL) {
						printf("Reallocation error\n");
						return NULL;
					}
				}
			}
			if (numOperators > 0) {
				newExp[subCounter]=delim;
				subCounter++;
				if (subCounter>=currLen) {
					newExp=safeRealloc(newExp, &currLen, sizeof(char));
					if (newExp==NULL) {
						printf("Reallocation error\n");
						return NULL;
					}

				}
			}
			numOperators++;
			newExp[subCounter]=exp[counter];
			subCounter++;
		}
		// otherwise- the thing in the expression is unecessary- ignore it
		else {
			if (other == 0) {
				other++;
				newExp[subCounter] = delim;
				subCounter++;
				if (subCounter >=currLen) {
					newExp = safeRealloc(newExp, &currLen, sizeof(char));
					if (newExp == NULL) {
						printf("Reallocation error\n");
						return NULL;
					}
				}
			}
		}
		counter++;
	}
	newExp[subCounter]='\0';
	return newExp; 
		
}
// determine type of data based on token
int detType(char *token) {
	// token is an operator
	if (strcmp(token, "+") == 0 || strcmp(token, "*") == 0 || strcmp(token, "(") ==0 || strcmp(token, ")") == 0 || strcmp(token, "-") == 0 || strcmp(token, "/") == 0)
		return 0;
	// check if token is a number- note atoi returns 0 on error though
	else if(atoi(token)!=0 || strcmp(token, "0") == 0) 
		return 1;
	else 
		return -1;
}

// determine if the left token (operator) has greater than or equal higher precdence than the right
int gtePrec(int left, int right) {
	// lower precedence cases- return 0
	if ((left == '+' || left == '-') && (right == '*' || right == '/' || right == '('))
		return 0;
	else if ((left == '*' || left == '/') && (right == '('))
		return 0;
	else 
		return 1;

}
