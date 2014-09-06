#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 20

int buf[MAX];
int items = 0, ci = 0, pi = 0;

pthread_mutex_t itemsLock;

pthread_cond_t cc, cp;

// Consumer does this
void *consumer(void *arg) {

	while (1) {

		// Acquire mutex first
		pthread_mutex_lock(&itemsLock);

		// Perhaps wait on condition variable
		if (items == 0) {

			pthread_cond_wait(&cc, &itemsLock);

		}

		// consume next item
		items--;
		printf("Consumed from index # %d\n", ci);
		ci = (ci + 1) % MAX;

		// Signal producer (if waiting)
		pthread_cond_signal(&cp);

		// Unlock mutex
		pthread_mutex_unlock(&itemsLock);

		sleep(3);

	}

}

// Producer does this
void *producer(void *arg) {

	while (1) {

		// Acquite mutex
		pthread_mutex_lock(&itemsLock);

		// Perhaps wait on condition variable
		if (items == MAX) {

			pthread_cond_wait(&cp, &itemsLock);

		}

		// produce next item
		items++;
		printf("Produced at index # %d\n", pi);
		pi = (pi + 1) % MAX;

		// signal consumer (if waiting)
		pthread_cond_signal(&cc);

		// release mutex
		pthread_mutex_unlock(&itemsLock);

		sleep(1);

	}

}

int main(int argc, char *argv[]) {

	pthread_t ptid, ctid;
	int ret;

	// Initialize mutex
	pthread_mutex_init(&itemsLock, NULL);

	// Initialize condition variables
	pthread_cond_init(&cc, NULL);
	pthread_cond_init(&cp, NULL);

	// create consumer thread
	ret = pthread_create(&ctid, NULL, consumer, NULL);

	if (ret != 0)
		perror("Consumer thread not created");

	// create producer thread
	ret = pthread_create(&ptid, NULL, producer, NULL);

	if (ret != 0)
		perror("Producer thread not created");

	pthread_join(ctid, NULL);
	pthread_join(ptid, NULL);

	return 0;

}
