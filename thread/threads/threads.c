#include <stdio.h>
#include <pthread.h>

void * run(void * args) {

	printf("Thread executing ... \n");

}

int main(int argc, char *argv[]) {

	pthread_t tid;
	int ret;

	ret = pthread_create(&tid, NULL, run, NULL);

	if (ret != 0)
		perror("Thread not created");

	pthread_join(tid, NULL);

	return 0;

}
