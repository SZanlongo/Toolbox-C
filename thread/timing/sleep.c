#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "timing.h"

void handler(int x) {

	printf("caught SIGINT\n");

}

int main(int argc, char *argv[]) {

	int i;

	struct timeval t1, t2;

	signal(SIGINT, handler);

	while (1) {

		// gettimeofday 1
		gettimeofday(&t1, NULL);

		sleep(1);

		// gettimeofday 2
		gettimeofday(&t2, NULL);

		// get difference
		struct timeval *diff = getDiff(t1, t2);

		printf("Loop iteration # %d; duration is secs = %llu; usecs = %llu\n",
				i++, (unsigned long long) diff->tv_sec,
				(unsigned long long) diff->tv_usec);

	}

	return 0;

}
