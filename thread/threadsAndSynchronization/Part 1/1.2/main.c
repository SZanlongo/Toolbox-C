#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS	200
#define PTHREAD_SYNC

int SharedVariable = 0;		//shared variable for all threads
pthread_mutex_t *shareLock;	//lock to prevent threads from accessing shared variable at the same time
pthread_barrier_t barr;	//barrier to synchronize all threads to the final value

void SimpleThread(void* x) {
	int which = (int) x;
	int num, val;

	for (num = 0; num < 20; num++) {//each thread increments and prints 20 times
		if (random() > RAND_MAX / 2)
			usleep(10);
#ifdef PTHREAD_SYNC					//sync threads
		pthread_mutex_lock(shareLock);//lock sharedVeriable so no other thread can use it
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;				//increment sharedVariable
		pthread_mutex_unlock(shareLock);//done using sharedVariable, so unlock
		pthread_barrier_wait(&barr);
#else
		val = SharedVariable;					//same thing without locks
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
#endif
	}
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
}

int main(int argc, char* argv[]) {
	int tNum = strtol(argv[1], NULL, 10);
	if (argc == 2 && argv[1] != 0) {		//check for correct input parameters
		pthread_t threads[tNum];				//array of threads
		shareLock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));//allocate mememory for lock
		pthread_mutex_init(shareLock, NULL);			//initialize the lock

		int i;
		if (pthread_barrier_init(&barr, NULL, tNum)) {//create barrier and check if that completed
			printf("not created \n");
			return -1;
		}

		for (i = 0; i < tNum; i++) {	//loop for the number of threads desired
			int t = pthread_create(&threads[i], NULL, (void*) SimpleThread,
					(void*) i);	//create threads and add them to the array
			if (t != 0) {
				printf("Error creating thread %d\n", i);//print if there was an error creating the thread
				return -1;
			}
		}

		for (i = 0; i < tNum; i++) {				//join all threads when done
			int x = pthread_join(threads[i], NULL);
			if (x != 0) {
				printf("Join failed on %d", i);
				return -1;
			}
		}
	} else {
		printf("Bad input\n");
	}
	return 0;
}

