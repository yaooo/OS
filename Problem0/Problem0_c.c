#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <time.h>


int num[1000000];
int len = 0;
clock_t startTime;
double elapsedTime;
char *readFileName;
FILE *writeFile;

typedef struct variables_struct{ int min,max,sum,count;}variables_struct;

void forkbfsdfs(int begin, int last, variables_struct *variables){
  
  if (begin == last){
    int num,number;
    FILE *fp = fopen("inf.txt", "r");

    int cs=0;
    while(fscanf(fp, "%d\n", &number) != EOF){
      if(begin == cs++){
      num = number;
      }
    }

    if (num < variables->min) {
      variables->min = num;
    }
    if (num > variables->max){
      variables->max = num;
    }

    variables->sum += num;
    variables->count ++;
    return;
  }

  int fd[2];

  pid_t waitpid(pid_t);
  pid_t childpid;

  pipe(fd);

  childpid= fork();

  if(childpid < 0){
    perror("Cannot create child process");
    exit(1);
  }

  else if (childpid== 0){
    
    fprintf(writeFile, "Hi I'm  %d and my parent is %d\n", getpid(), getppid());
    close(fd[0]);
    forkbfsdfs((begin+last)/2 + 1, last, variables);
    write(fd[1], variables, sizeof(variables_struct));
    exit(0);
  }
  else{
    close(fd[1]);
    forkbfsdfs(begin, (begin+last)/2, variables);
    variables_struct new_variables;
    read(fd[0], &new_variables, sizeof(variables_struct));
    
    if (new_variables.min < variables->min){
       variables->min = new_variables.min;
      }
    if (new_variables.max > variables->max){
       variables->max = new_variables.max;
      }

    variables->sum += new_variables.sum;
    variables->count += new_variables.count;
    waitpid(childpid);
  }
}


int main(){
  writeFile = fopen("out.txt", "w+");
  FILE *fp = fopen("Input100k.txt", "r");
  int count = 0;
  int numbers;

  while(fscanf(fp, "%d\n", &numbers) != EOF){
    count++;
  }

  fclose(fp);
  variables_struct variables = {.min = INT_MAX, .max = INT_MIN, .sum = 0, .count = 0};
  
    startTime = clock();

  forkbfsdfs(0, count-1, &variables);

  fprintf(writeFile, "minimum: %d\n", variables.min);
  fprintf(writeFile, "maximum: %d\n", variables.max);
  fprintf(writeFile, "sum: %d\n",variables.sum);

  fclose(writeFile);
 elapsedTime = (clock() - startTime);
    printf("Total time: %0.3f ticks from Problem 0 part b.(1 tick = 1/1000000s)\n", elapsedTime);
    
    return(0);
}