#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#define NUMPARTS 4
#define NUMLEVELS 3
#define MAXPROCS 1000
#define SLEEPTIME 30

// modification of Problem 0, part c so that IPC used is via signals
// we wish to also include the specifications of part b and part c

// note that because communication is done via signals, we must get rid of communication between parent/child processes if we wish to allow the user to enter their own
// signal

int num[1000000];
int len = 0;
clock_t startTime;
double elapsedTime;
int globalMin=1000, globalMax = -1, globalSum=0;
int currentPID=0;
int pidList[MAXPROCS]; // list of PIDS: -1 indicates that the process is not in use, 0 indicates that it is finished, any other number indicates that the process with PID stored in pidList is still executing
int currProcs=0;
int end=0;
int sigRec=-1;
int option2=0;

// data structure for sending data between processes
typedef struct statInfo{
	int iData1;
	int iData2;
	int iData3;
	int iData4;
}statInfo; 

// handler function that the PARENT process should be receiving
void handlerParent(int signum, siginfo_t *si, void *ucontext) {
	// si->si_value = of type union sigval
	statInfo *siPtr = (statInfo *)(((si->si_value).sival_ptr));
	int tempMin = siPtr->iData1;
	int tempMax = siPtr->iData2;
	int tempSum = siPtr->iData3;
	int currPIDIndex = siPtr->iData4;
	
	printf("child process terminated. At pid %d, ppid %d, the min, max, sum received was %d, %d, %d\n", getpid(), getppid(), tempMin, tempMax, tempSum);
	if (tempMin != -1 && tempMax!=-1 && tempSum != -1 && currPIDIndex!=-1) {
		if (tempMin < globalMin)
			globalMin = tempMin;
		if (tempMax > globalMax)
			globalMax = tempMax;
		globalSum=globalSum+tempSum;
		pidList[currPIDIndex] = 0;
	}
	return;
}

// handler function that the parent process sends to child process(es)
void handlerChild(int signum, siginfo_t *si, void *ucontext) {
	// send as a signal information about the "slow" child process
	printf("this process, pid %d is taking too long. Setting the ignore/end variable.\n", getpid());
	end=1;
}

// handler function to update global variables currProcs
void handlerUpdate(int signum, siginfo_t *si, void *ucontext) {
	if (signum==SIGUSR2) {
		printf("process %d is updating its pidList\n", getpid());
		pidList[currProcs] = (si->si_value).sival_int;
		currProcs++;	
	}
	else {
		printf("this handler could be used for other signals as well but we haven't defined them yet\n");
	}
}

// handler function for when user sends a signal to a particular process(es)
void handlerUser(int signum) {
	// ctrl- c corresponds to SIGINT

	printf("A signal was caught for process %d, ppid %d. The particular signal value was...\n", getpid(), getppid());
	sigRec = signum;
	if (signum == SIGINT) 
		printf("%d, corresponding to SIGINT\n", signum);	
	if (signum == SIGQUIT)
		printf("%d, corresponding to SIGQUIT\n", signum);	
	if (signum == SIGSTOP)
		printf("%d, corresponding to SIGSTOP\n", signum);	
	if (signum == SIGTSTP)
		printf("%d, corresponding to SIGTSTP\n", signum);	
	if (signum == SIGABRT)
		printf("%d, corresponding to SIGABRT\n", signum);	
	if (signum == SIGTERM)
		printf("%d, corresponding to SIGTERM\n", signum);	
	if (signum == SIGKILL)
		printf("%d, corresponding to SIGKILL\n", signum);	
	// default action is to be taken
	if (option2==1) { 
		sigRec = -1;
		signal(signum, SIG_DFL);
		printf("pid %d, ppid %d is doing original action for signal %d\n", getpid(), getppid(), signum);
		kill(getpid(), signum);
	}
	// check for pending signals
	if (option2 == 3) { 
		sigset_t pending;
		sigemptyset(&pending);
		sigpending(&pending);
		if (sigismember(&pending, SIGINT))
			printf("pid %d ppid %d is blocking SIGINT\n", getpid(), getppid());
		if (sigismember(&pending, SIGQUIT))
			printf("pid %d ppid %d is blocking SIGQUIT\n", getpid(), getppid());
		if (sigismember(&pending, SIGTSTP))
			printf("pid %d ppid %d is blocking SIGTSTP\n", getpid(), getppid());
	}

	return;
}

// min, max, sum functions
int min(int *x, int begin, int end) {
    int mintemp = x[0];
    for (int i = begin;i < end; i++) {
        if(x[i] < mintemp) {
            mintemp = x[i];
        }
    }
    return mintemp;
}
int max(int *x, int begin, int end) {
    int maxtemp = x[0];
    for (int j = begin; j < end; j++) {
        if(x[j] > maxtemp) {
            maxtemp = x[j];
        }
    }
    return maxtemp;
}
int sum(int *x, int begin, int end) {
    int sumtemp = 0;
    for (int k = begin; k < end; k++) {
        sumtemp = sumtemp + x[k];
    }
    return sumtemp;
}
int main(int argc, char* argv[]){
    if (argc!=2) {
	    printf("Invalid arguments. \nUSAGE: ./Part1_a.exe <input file name>\n");
	    return 0;
    }

    char inputFile[1000];
    strcpy(inputFile, argv[1]);

    FILE *fp = fopen(inputFile, "r");

    if (fp==NULL) {
    	printf("unable to open %s\n", inputFile);
	return 0;
    }

    FILE *output = fopen("debug.txt", "w+");
    if (output==NULL) {
	    printf("unable to open output file\n");
            return 0;	
    }
    
    int count = 0;
    int numbers; //buffer for each line

    // set signals for parts a and b here 
    struct sigaction parentAction, childAction, updateGlobal;
    sigemptyset(&parentAction.sa_mask);
    // parentAction.sa_handler = handlerParent;
    parentAction.sa_sigaction = handlerParent;
    parentAction.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &parentAction, NULL); 

    sigemptyset(&childAction.sa_mask);
    childAction.sa_sigaction = handlerChild;
    childAction.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &childAction, NULL);

    sigemptyset(&updateGlobal.sa_mask);
    updateGlobal.sa_sigaction = handlerUpdate;
    updateGlobal.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &updateGlobal, NULL);


    // read numbers into array
    while(fscanf(fp, "%d\n", &numbers) != EOF){
        num[count] = numbers;
        count++;
    }
    fclose(fp);  

    printf("Number of elements: %d\n", count);
    fprintf(output, "Number of elements: %d\n", count);
    // printf("%d\n", max(num, 0, count));


    // original definition from "problem0_c.c"
    /*
    int partitions = 8;
    int currentPart = 0;
    int levels = 3;
    int section = count/partitions;
    */

    int comp=0;
    int cnter=0;
    int partitions = 0; // make it a random factor of the size of the input file
    /*
    srand(time(NULL));
    while (!comp) {
	partitions=(rand() % count)+1;
	fprintf(output, "Iteration %d, partitions: %d\n", cnter, partitions);
	cnter++;
	if ((count%partitions)==0)
		comp = 1;
	if (cnter>=1000) { // taking way too long
		fprintf(output, "took too long, set to 10 partitions\n");
		partitions=10;
		comp=1;
	}
    }
    */
    partitions=NUMPARTS;
    int currentPart=0; // iterates through the current number of partitions done
    int totalPart=0; // iterates through the current number of partitions done
    int levels=rand() % 6; // have levels to be some arbitrary number- note that 
    //  2^(levels) -1 processes in total will be created.
    int workPerChld=(rand() % (partitions))+1; // state that each child process will operate on some random number of partitions
    // need to make sure that workPerChld*total number of processes - 1 >= total number of partitions
    cnter=0;
    comp=0;

    // part c related signal stuff
    // signals: i) SIGINT, ii) SIGQUIT, iii) SIGSTOP, iv) SIGTSTP, v) SIGABRT, vi) SIGTERM, vii) SIGKILL
    struct sigaction userSigs;
    sigset_t set;
    sigemptyset(&userSigs.sa_mask);
    userSigs.sa_handler = handlerUser;
    // userSigs.sa_flags = 0;
    printf("Handling the seven specified signals: \nSIGINT = %d\nSIGQUIT = %d\nSIGSTOP = %d\nSIGTSTP = %d\nSIGABRT = %d\nSIGTERM =%d\nSIGKILL = %d\n If an error message did not show up, then the signal was handled.\n", SIGINT, SIGQUIT, SIGSTOP, SIGTSTP, SIGABRT, SIGTERM, SIGKILL);
    if (sigaction(SIGINT, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGINT (unexpected) \n");
    if (sigaction(SIGQUIT, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGQUIT (unexpected)\n");
    if (sigaction(SIGSTOP, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGSTOP (expected for SIGSTOP and SIGKILL in general)\n");
    if (sigaction(SIGTSTP, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGTSTP (unexpected)\n");
    if (sigaction(SIGABRT, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGABRT (unexpected)\n");
    if (sigaction(SIGTERM, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGTERM (unexpected)\n");
    if (sigaction(SIGKILL, &userSigs, NULL) == -1)
	    printf("Couldn't invoke handler for SIGKILL (expected for SIGSTOP and SIGKILL in general)\n");

    printf("If you'd like to enter your own option to enter a signal of choice, enter 0\nTo send signals (from process itself) to all processes in process groups, enter 1.\nTo send signals (from process itself) to only itself enter any other number (like 2)\n");
    int option=0;
    scanf("%d", &option);
    printf("%d entered.\n", option);

    printf("Enter which section of part c) you'd like done (enter 1, 2, or 3)\ni): 1\nii): 2\niii): 3\n");
    scanf("%d", &option2);
    printf("%d entered\n");
    if (option2==1) 
	printf("i) default handlers used, so signals will take on their default action\n");
    else if (option2==2) 
	printf("ii) we will give back information about each signal in this case\n");
    else if (option2==3) {
	printf("iii) we have SIGINT, SIGQUIT, and SIGTSTP blocked\n");
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGTSTP);
	sigprocmask(SIG_BLOCK, &set, NULL);  // so we shouldn't be able to receive these signals more than once at a time
    }
    else 
	printf("invalid option entered; we'll set the option to do the actions of ii).\n");

    // determining work done per child process
    while(!comp) {
        workPerChld=(rand() % (partitions))+1;
	fprintf(output, "Iteration %d, workPerChld: %d\n", cnter, workPerChld);
	cnter++;
	if (workPerChld*pow(2, levels)>= partitions) 
		comp=1;
	if(cnter>=1000 || levels==0) { // taking too long OR levels==0, which is possible
		fprintf(output, "took too long, set to partitions done per child process\n");
		comp=1;
		workPerChld = partitions;
	}
    }

    int section = count/partitions; // each "section" describes exactly how many elements will be in each of the partitions

    // we enforce the following
    levels = NUMLEVELS;
    partitions=NUMPARTS;

    printf("Total number of elements: %d\nTotal number of levels: %d\nTotal number of partitions: %d\nPartitions to be done per child process: %d\n", count, levels, partitions, workPerChld);
    fprintf(output, "Total number of elements: %d\nTotal number of levels: %d\nTotal number of partitions: %d\nPartitions to be done per child process: %d\n", count, levels, partitions, workPerChld);

    // printf("count; section size; partitions...%d;%d;%d\n", count, section, partitions); //proof of concept
   // fprintf(output,"\ncount; section size; partitions...%d;%d;%d\n", count, section, partitions); 


    int partMin = 1000, partMax = -1, partSum = 0;
    int chldID=-1; // to represent child processes
    int origPPID = getppid(); // original parent's ppid
    union sigval stats, slowInfo, currPID;
    cnter=0;
    for(cnter;cnter<MAXPROCS;cnter++) {
	pidList[cnter] = -1;
    }
    // statInfo *statPtr;
    statInfo si;
    si.iData1=-1;
    si.iData2=-1;
    si.iData3=-1;
    si.iData4=-1;

    comp=0;
    startTime = clock();

    if (levels==0) { // case where only the parent process does work
      int minSect, maxSect, sumSect;
      printf("Only parent process is doing work.\n");	
      for(totalPart;totalPart<partitions;totalPart++) {
	      minSect=min(num, totalPart*section, (totalPart+1)*section);	
	      maxSect=max(num, totalPart*section, (totalPart+1)*section);	
	      sumSect=sum(num, totalPart*section, (totalPart+1)*section);	
	      if (minSect < globalMin)
		      globalMin=minSect;
	      if (maxSect > globalMax)
		      globalMax = maxSect;
	      globalSum = globalSum+sumSect;
      }
    }
    for (int i = 0; i < levels; ++i)
    {
	chldID=fork();
	if (totalPart>=partitions || comp)
		break;
        if (!chldID) //creates the randomly generated process tree -- each left child creates 3 additional children until it reaches max
        {
		// spin in place if option == 0
		if (option==0) {
                        printf("Option for user to send signals from command line invoked\n");
                        printf("\nProcess %d will sleep for %d seconds to allow for user to kill the program\nTo send signals in linux: kill [-signal number] PID; note if PID is 0, the signal is sent to all processes in the process group.", getpid(),SLEEPTIME);
                        printf("Note: entering ctrl-c from command line sends SIGINT to all processes in process group\n");
                        // user enters something
                        sleep(SLEEPTIME);
                        // printf("Signal with value %d received\n", sigRec);
                        // wait(NULL);
                        exit(0);
                }

	    int minSect, maxSect, sumSect, tempMin = 1000, tempMax = -1, tempSum = 0, tempPart = totalPart;
            fprintf(output, "Newly created child and doing an action: PID: %d, Parent PID: %d\n", getpid(), getppid());
	    currentPart=0;
	    // printf("pid %d, ppid %d about to do work\n", getpid(), getppid());
	    // now each child process will do workPerChld number of sections
	    for(currentPart;currentPart<workPerChld;currentPart++) {
	      minSect=min(num, totalPart*section, (totalPart+1)*section);	
	      maxSect=max(num, totalPart*section, (totalPart+1)*section);	
	      sumSect=sum(num, totalPart*section, (totalPart+1)*section);	
	      if (minSect < tempMin)
		      tempMin = minSect;
	      if (maxSect > tempMax)
		      tempMax = maxSect;
	      tempSum=tempSum+sumSect;
	      // printf("pid %d, ppid %d about to update statPtr\n", getpid(), getppid());
	      /*
              statPtr->iData1 = tempMin;
              statPtr->iData2 = tempMax;
              statPtr->iData3 = tempSum;
	      */
	      si.iData1=tempMin;
	      si.iData2=tempMax;
	      si.iData3=tempSum;
	      // printf("pid %d, ppid %d updated statPtr\n", getpid(), getppid());
	      totalPart++; // somehow indicate to parent process that totalPart partitions have been completed...
	      if (totalPart >= partitions || comp) {
		      break;
		      comp=1;
	      }
	    }
	    printf("Process %d, parent %d, min=%d, max=%d, sum=%d for partitions %d through %d\n", getpid(), getppid(), tempMin, tempMax, tempSum, tempPart, totalPart);

	    // child process will do workPerChld partitions
	    /*
            minSect = min(num, currentPart*section, (currentPart+1)*section);
            maxSect = max(num, currentPart*section, (currentPart+1)*section);
            sumSect = sum(num, currentPart*section, (currentPart+1)*section);
            currentPart++;
	    printf("Process %d, parent %d, max=%d, min=%d, sum=%d\n", getpid(), getppid(), maxSect, minSect, sumSect);
	    */
	    /*
            write(fdmin[1], &minSect, sizeof(minSect));
            write(fdmax[1], &maxSect, sizeof(maxSect));
            write(fdsum[1], &sumSect, sizeof(sumSect));
	    */
	    // fprintf(output, "Process %d, parent %d, max=%d, min=%d, sum=%d\n", getpid(), getppid(), partMax, partMin, partSum);
        }
        else{
	  
          fprintf(output, "In for loop waiting on some child CHLDID %d: PID: %d, Parent PID: %d\n", chldID, getpid(), getppid());
          printf("Waiting some child CHLDID %d: PID: %d, Parent PID: %d, currently on partition %d\n", chldID, getpid(), getppid(), totalPart);

	  // tell parent process/itself that some process is underway
	  currPID.sival_int = chldID;
	  if (getppid()!=origPPID && option!=0) 
          	sigqueue(getppid(), SIGUSR2, currPID);
	  pidList[currProcs] = chldID;
	  currProcs++; 
	  
	  
	  // switching wait(NULL) and sleep(3) should give the results of Part1_a and Part1_b respectively
          // wait(NULL); 
	  
	  // sleep(3);
	  if (getppid()!=origPPID) {
	  	if (option==0) {
			printf("Option for user to send signals from command line invoked\n");
	  		printf("\nProcess %d will sleep for %d seconds to allow for user to kill the program\nTo send signals in linux: kill [-signal number] PID; note if PID is 0, the signal is sent to all processes in the process group.", getpid(),SLEEPTIME);
			printf("Note: entering ctrl-c from command line sends SIGINT to all processes in process group\n");
			// user enters something
	  		sleep(SLEEPTIME);
			printf("Signal with value %d received\n", sigRec);
			// wait(NULL);
			
			exit(0);
	  	}
	  	if (option == 1) {
			printf("Option to send the signals from the program to itself to all processes in process group selected\n");
			printf("pid %d, ppid %d is sending SIGINT or ctrl-C to all processes\n", getpid(), getppid());
			kill(0, SIGINT);
			printf("Value of signal received from SIGINT: %d\n", sigRec);

			printf("Sending SIGQUIT to all processes\n");
			kill(0, SIGQUIT);
			printf("Value of signal received from SIGQUIT: %d\n", sigRec);

			// toggle SIGSTOP on/off depending on preferences
			printf("Since SIGSTOP can't be handled we won't send it to all processes\n");
			/*
			printf("Sending SIGSTOP to all processes\n");
			kill(0, SIGSTOP);
			printf("Value of signal received from SIGSTOP: %d\n", sigRec);
			*/

			printf("Sending SIGTSTP to all processes\n");
			kill(0, SIGTSTP);
			printf("Value of signal received from SIGTSTP: %d\n", sigRec);

			printf("Sending SIGABRT to all processes\n");
                        kill(0, SIGABRT);
                        printf("Value of signal received from SIGABRT: %d\n", sigRec);

			printf("Sending SIGTERM to all processes\n");
                        kill(0, SIGTERM);
			printf("Value of signal received from SIGTERM: %d\n", sigRec);

			// toggle SIGKILL on/off depending on preferences
			 printf("Since SIGKILL can't be handled we won't send it to all processes\n");
                        /*
                        printf("Sending SIGKILL to all processes\n");
                        kill(0, SIGKILL);
                        printf("Value of signal received from SIGKILL: %d\n", sigRec);
                        */
			printf("pid %d, ppid %d, which was the sender of the signals, is exiting\n", getpid(), getppid());
			wait(NULL);
			exit(0);

	  	}
	  	else {
			printf("Option to send the signals from the program to itself to only itself selected\n");
			printf("pid %d, ppid %d is sending SIGINT or ctrl-C to itself\n", getpid(), getppid());
			kill(getpid(), SIGINT);
			printf("Value of signal received from SIGINT: %d\n", sigRec);

			printf("Sending SIGQUIT to itself\n");
			kill(getpid(), SIGQUIT);
			printf("Value of signal received from SIGQUIT: %d\n", sigRec);

			// toggle SIGSTOP on/off depending on preferences
			printf("Since SIGSTOP can't be handled we won't send it\n");
			/*
			printf("Sending SIGSTOP to itself\n");
			kill(getpid(), SIGSTOP);
			printf("Value of signal received from SIGSTOP: %d\n", sigRec);
			*/

			printf("Sending SIGTSTP to itself\n");
			kill(getpid(), SIGTSTP);
			printf("Value of signal received from SIGTSTP: %d\n", sigRec);

			printf("Sending SIGABRT to itself\n");
                        kill(getpid(), SIGABRT);
                        printf("Value of signal received from SIGABRT: %d\n", sigRec);

			printf("Sending SIGTERM to itself\n");
                        kill(getpid(), SIGTERM);
			printf("Value of signal received from SIGTERM: %d\n", sigRec);

			// toggle SIGKILL on/off depending on preferences
			 printf("Since SIGKILL can't be handled we won't send it\n");
                        /*
                        printf("Sending SIGKILL to itself\n");
                        kill(getpid(), SIGKILL);
                        printf("Value of signal received from SIGKILL: %d\n", sigRec);
                        */
			printf("pid %d, ppid %d, which was the sender of the signals, is exiting\n", getpid(), getppid());
			wait(NULL);
			exit(0);
	  	}
	  }	
	  printf("Nothing was entered for pid %d, ppid %d\n", getpid(), getppid());
	  if (pidList[currProcs-1] != 0) {
		// send signal to all processes, or at least to the child process in question
		printf("process %d is taking too long\n", pidList[currProcs-1]);
		slowInfo.sival_int = chldID;
		if (option!=0)
			sigqueue(chldID, SIGUSR1, slowInfo);
		
	  }
	  printf("done waiting\n");
          // int min, max, sum,fSum;
	  /*
          read(fdmin[0], &min, sizeof(min));
          read(fdmax[0], &max, sizeof(max));
          read(fdsum[0], &sum, sizeof(sum));
          printf("Max = %d\n", getpid(), max);
          printf("Min = %d\n", getpid(), min);
          printf("Sum = %d\n", getpid(), sum);
          fSum += sum;
	  */
        }
    }
    // we want to find these statistics for the original process only
    /*
    elapsedTime = (clock() - startTime);
    printf("Process %d, parent %d, max=%d, min=%d, sum=%d\n", getpid(), getppid(), globalMax, globalMin, globalSum);
    printf("Process %d, parent %d\n", getpid(), getppid());
    fprintf(output, "Process %d, parent %d, max=%d, min=%d, sum=%d\n", getpid(), getppid(), partMax, partMin, partSum);
    printf("Total time of said process: %0.3f ticks from Part 1.a.(1 tick = 1/1000000s)\n", elapsedTime);
    fprintf(output, "Total time of said process: %0.3f ticks from Part 1.a.(1 tick = 1/1000000s)\n\n", elapsedTime);
    fprintf(output, "Out of loop: PID: %d, Parent PID: %d\n", getpid(), getppid());
    */

    // the process took too long, so we'll ignore its contents
    if (end) {
	printf("this process pid %d, ppid %d took too long so we'll ignore it\n", getpid(), getppid()); 
	fprintf(output, "pid %d, ppid %d took too long\n", getpid(), getppid());
	return 0;
    }
    if (option==0 && getppid()!=origPPID)
	sleep(SLEEPTIME);    

    // if the current process was a child of the original...
    if (getppid()!=origPPID) {
      fprintf(output, "Process %d, parent %d about to send data\n", getpid(), getppid());
      si.iData4 = currProcs;
      pidList[currProcs]=0;
      // stats.sival_ptr = (void *)statPtr;
      stats.sival_ptr = (void *)(&si);
      if (option!=0)
      	sigqueue(getppid(), SIGCHLD, stats);
    }
    else {
	wait(NULL); // added so that the parent process will block, waiting for the child processes
	printf("complete\n");
	printf("overall min: %d\n", globalMin);
	printf("overall max: %d\n", globalMax);
	printf("overall sum: %d\n", globalSum);
	printf("Processes that did not finish on time\n");
	cnter=0;
	int totalProcs=0;
	int trouble=0;
	for(cnter;cnter<1000;cnter++) {
		if (pidList[cnter]!=-1)
			totalProcs++;
		if (pidList[cnter]>0) {
			printf("Process %d had trouble\n", pidList[cnter]);
			trouble++;
		}
			
	}
	if (trouble==0)
		printf("No processes took too long\n");
	printf("Total number of processes created: %d\n", totalProcs);
    	elapsedTime = (clock() - startTime);
        printf("Total time of all tasks: %0.3f ticks from Part 1.a.(1 tick = 1/1000000s)\n", elapsedTime);
    }
    fclose(output);
    return(0);
}
