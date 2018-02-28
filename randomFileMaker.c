#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(){
		srand(time(NULL));   // should only be called once
	int randomNumber;
	int fileSize = 1000;

	FILE *output = fopen("inputRandom1000.txt", "w+");

	for (int i = 0; i < fileSize; ++i)
	{
		randomNumber = rand() % 1000;
		fprintf(output, "%d\n", randomNumber);
	}
	return 0;
}