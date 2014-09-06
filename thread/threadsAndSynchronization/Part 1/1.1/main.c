#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_THREADS	200

int SharedVariable = 0;	//this holds the shared number that each thread will print

void SimpleThread(void* x) {
	int which = (int) x;	//cast x to int
	int num, val;
	for (num = 0; num < 20; num++) {//for each thread, increment and print 20 times
		if (random() > RAND_MAX / 2)
			usleep(10);
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
	}
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);//print final value, this might be different across threads
}

int main(int argc, char* argv[]) {
	int tNum = strtol(argv[1], NULL, 10);	//number of threads
	pthread_t threads[tNum];		//array to hold threads
	int i;
	for (i = 0; i < tNum; i++) {			//loop while < number of threads
		int t = pthread_create(&threads[i], NULL, (void*) SimpleThread,
				(void*) i);	//create the new thread and add it to the array
		if (t != 0) {
			printf("Error creating thread %d\n", i);//if there is an error creating the thread, alert the user
			return -1;
		} else {
			printf("Created new thread %d\n", i);
		}
	}
	for (i = 0; i < tNum; i++) {						//join threads when done
		int x = pthread_join(threads[i], NULL);
		if (x != 0) {
			printf("Join failed on %d", i);
			return -1;
		}
	}
}

