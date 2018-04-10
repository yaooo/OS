sem_t access;	//binary semaphore - only allows access to either the employee or the cleaner (mutex condidtion)

int N; //N = number of <employees>
sem_t keychain;	//initialize keychain value to the number of employees (N)

//wait: to check if worker can go into the office space (increment semaphore count if successful)
//signal: open up a space in office (decrement semaphore count if successful)

void employee(){
	while(true){
		wait(&access);		//switches access to ONLY employees
		wait(&keychain);	//takes one key from the keychain (decrements the value)
		working_window();
		signal(&keychain)	//surrenders key back to keychain (increment the value)
		int numberOfKeysTaken = 0; //initialize some counter
		sem_getvalue(&keychain, &numberOfKeysLeft)
		if (numberOfKeysLeft == N)	//if there is an available key for every employee, that means there are no employees left
		{
			signal(&access);	//give up access to room. The next worker in queue will get access for next iteration
		}
		break_time();
	}
}

void cleaner(){
	while(true){
		wait(&access);	//switches access to ONLY cleaners
		clean_office();
		signal(&access);
		break_time();
	}
}


