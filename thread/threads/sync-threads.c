#include <stdio.h>
#include <pthread.h>

#define MAX 500000

// global 
int items = 0;
pthread_mutex_t itemsLock;

void * increment(void * args) {

	int tid = (int) args;

	int counter;

	printf("Thread [%d]: Starting execution ...\n", tid);

	printf("Thread [%d]: Initial value of items = %d\n", tid, items);

	// increment items
	for (counter = 0; counter < MAX; counter++) {

		// obtain mutex lock
		pthread_mutex_lock(&itemsLock);

		items++;

		// release mutex lock
		pthread_mutex_unlock(&itemsLock);

	}

	printf("Thread [%d]: Final value of items = %d\n", tid, items);

	// cleanup thread resources
	// this can be used to return information to joining threads

	pthread_exit(NULL);

}

int main(int argc, char *argv[]) {

	pthread_t tid[10];
	int ret, i;

	pthread_mutex_init(&itemsLock, NULL);

	for (i = 0; i < 10; i++) {

		ret = pthread_create(&tid[i], NULL, increment, i + 1);

		if (ret != 0)
			perror("Thread not created");

	}

	for (i = 0; i < 10; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("Final items = %d\n", items);

	// Cleanup  mutex resources

	pthread_mutex_destroy(&itemsLock);

	return 0;

}
