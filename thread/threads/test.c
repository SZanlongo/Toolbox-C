#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	void *addr = malloc(10);

	printf("%p\n", addr);

	free(addr);
	free(addr);
	//free(addr+1);

	printf("%s\n", (char *) addr);

	return 0;

}
