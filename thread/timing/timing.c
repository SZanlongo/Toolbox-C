#include <stdlib.h>

#include <sys/time.h>

struct timeval * getDiff(struct timeval t1, struct timeval t2) {

	struct timeval * diff = (struct timeval *) malloc(sizeof(struct timeval));

	// This is not correct
	// diff->tv_sec = t2.tv_sec - t1.tv_sec;
	// diff->tv_usec = t2.tv_usec - t1.tv_usec;

	long long diffUS = (t2.tv_sec * 1000000 + t2.tv_usec)
			- (t1.tv_sec * 1000000 + t1.tv_usec);

	diff->tv_sec = diffUS / 1000000;
	diff->tv_usec = diffUS % 1000000;

	return diff;

}
