#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define PG_SIZE 4096
#define SLAB_SIZE PG_SIZE*4

int main(int argc, char *argv[]) {

	// Works for Linux
	// int fd = open("/dev/zero", O_RDWR);

	// Workaround for a Mac
	// Map a file filled with zeroes
	int fd = open("LARGEFILE", O_RDWR);

	void *region = mmap(NULL, PG_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd,
			4096);

	if (region == MAP_FAILED)
		perror("mmap");

	printf("%s", (char *) region);

	strcpy(region, "MODIFIED");

	return 0;

}
