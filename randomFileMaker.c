#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	// srand(time(NULL));   // should only be called once
	// int randomNumber;
	int fileSize = atoi(argv[1]);

	FILE *output = fopen("input.txt", "w+");

	// for (int i = 0; i < fileSize; ++i){
	// 	randomNumber = rand() % 1000;
	// 	if (i == fileSize-1)
	// 	{
	// 		fprintf(output, "%d", randomNumber);
	// 		break;
	// 	}
	// 	fprintf(output, "%d\n", randomNumber);
	// }

	for (int i = 0; i < fileSize; ++i)
	{
		if (i == fileSize-1)
		{
			fprintf(output, "%d", i);
			break;
		}
		fprintf(output, "%d\n", i);
	}
	return 0;
}