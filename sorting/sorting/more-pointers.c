#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 1

#ifdef X

#define LEN 10

int g1 = 9, g2 = 1;

void myFree(void *h) {

	free(h);

}

int main(int argc, char *argv[]) {

	int x = 1, y = 2, z[LEN];

	int *ip;

	char *h = (char *) malloc(10);
	strcpy(h, "test string");

	ip = &x;

	printf("Address of x is %p\n", ip);
	printf("Address of y is %p\n", &y);
	printf("Address of g1 is %p\n", &g1);
	printf("Address of g2 is %p\n", &g2);
	printf("Value of h is %p\n", h);
	printf("Address of h is %p\n", &h);

	//myFree(h);

	char *k = (char *) malloc(10);
	strcpy(k, "new string");

	printf("Accessing string after free %s\n", h);

	return 0;

}

#else

#endif

