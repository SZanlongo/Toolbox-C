#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <strings.h>

#include "bits.h"

#define PG_SIZE 4096
#define SLAB_SIZE 16*PG_SIZE

// some possibilities ...
// #define BM_SIZE(x) SLAB_SIZE/x/(8*sizeof(int))

// slab allocation (memory->caches->slabs)
//       - 9 allocation units (8,16,32,64,128,256,512,1024,2048)
// region-based allocation (direct mmap >= 2K

typedef struct __slab {
	void *addr;
	// int bm[BM_SIZE(8)];
	int bm[SLAB_SIZE / 8 / (8 * sizeof(int))];

// old version
// int *bm;
// struct __slab *next;
} slab;

typedef struct __region {
	int size;
	void *addr;
	struct __region *next;
} region;

typedef struct {
	int alloc_unit;
	slab S;
// old version
// slab *S;
} cache;

typedef struct {
	cache C[9];
	region *R;
} memory;

memory M; // Global memory 
int fd; // zero mapped file descriptor

void init_memory() {

	int i;

	// initialize region list
	M.R = NULL;

	// initialize the caches
	for (i = 0; i < 9; i++) {
		M.C[i].alloc_unit = 8 << i;
		M.C[i].S.addr = NULL;
		bzero(M.C[i].S.bm, SLAB_SIZE / 8 / 8);
	}

	fd = open("ZEROES", O_RDWR);

	// check fd is OK ... skipping for now

}

void *allocateCache(unsigned int size) {

	// 1. Select the cache
	int ci = 0;

	while ((size - 1) >> (ci + 3)) {
		ci++;
	}

	// 2. Find a slot
	int bmi = 0;
	int pos = -1;

	do {
		pos = find_zero_bit(M.C[ci].S.bm[bmi]);
		if (pos == -1)
			bmi++;
		else
			break;
	} while (bmi < SLAB_SIZE / M.C[ci].alloc_unit / (8 * sizeof(int)));

	int slot;

	if (pos == -1) {
		// 3a. Slot not found -- Return ??
		return NULL;
	}

	// 3b. Slot found

	// mark slot used in bitmap
	set_bit(&M.C[ci].S.bm[bmi], pos);

	slot = bmi * 32 + (pos - 1);

	if (M.C[ci].S.addr == NULL) {

		// create a slab using mmap
		M.C[ci].S.addr = mmap(NULL, SLAB_SIZE, PROT_READ | PROT_WRITE,
				MAP_PRIVATE, fd, 0);

		if (M.C[ci].S.addr == MAP_FAILED) {
			perror("mmap failed:");

			// this should be : return NULL;
			exit(1);
		}

	}

	// 4. Return: slab_base_addr + (slot)*alloc_unit

	return M.C[ci].S.addr + slot * M.C[ci].alloc_unit;

}

void *allocateRegion(unsigned int size) {

	void *addr = mmap(NULL, size + sizeof(region), PROT_READ | PROT_WRITE,
			MAP_PRIVATE, fd, 0);

	if (addr == MAP_FAILED) {
		perror("mmap failed:");
		exit(1);
	}

	region *newRegion = addr;
	newRegion->size = size;
	// newRegion->addr = newRegion + 1;
	newRegion->addr = (char *) addr + sizeof(region);
	newRegion->next = M.R;
	M.R = newRegion;

	printf("%p %p %p\n", newRegion, newRegion + 1,
			(region *) addr + sizeof(region));

	return newRegion->addr;

}

void *allocate(unsigned int size) {

	if (size > (8 << 8))
		return allocateRegion(size);
	else
		return allocateCache(size);

}

int deallocateRegion(void *ptr) {

	// check if ptr is a valid allocation
	// first, I will check regions
	region *rP = (region *) ptr - 1;
	region *candidate = M.R;
	region *prevCandidate = NULL;

	while (candidate != NULL) {

		if (candidate == rP) {

			printf("Deallocating %p\n", ptr);
			// found it in my regions; fix list
			if (prevCandidate == NULL) {
				M.R = candidate->next;
			} else {
				prevCandidate->next = candidate->next;
			}

			// munmap region
			munmap(candidate, candidate->size + sizeof(region));
			return 1;
		}
		prevCandidate = candidate;
		candidate = candidate->next;
	}

	// if not found, don't do anything
	printf("Allocation %p not found!\n", ptr);
	return 0;

}

void deallocateCache(void *ptr) {

	// HOMEWORK -- fill this out

	// next, check the caches

	// check if ptr is in the range
	// (slab_addr, slab_size)
	// for each slab in each cache

	// check if ptr is the starting address
	// of some slot in the slab that matches

	// if yes, found it in my caches;
	// reset bit in bitmap;
	// munmap slab? -- depends on the rest
	// of the bitmap

	// else, allocation doesn't exist
	// print a message "Allocation doesn't exist for address ptr
	// crash the program ; exit;

}

void deallocate(void *ptr) {

	if (deallocateRegion(ptr) == 1)
		return;
	else
		deallocateCache(ptr);

}

int main(int argc, char *argv[]) {

	init_memory();

	// allocate 3000 bytes
	void * addr1 = allocate(3000);
	printf("Allocated 3000 @ %p\n", addr1);
	bzero(addr1, 3000);
	printf("Allocation 1: \"%s\"\n", (char *) addr1);

	void *addr2 = allocate(10000);
	printf("Allocated 10000 @ %p\n", addr2);
	strcpy(addr2, "Hello, world!");
	printf("Allocation 2: \"%s\"\n", (char *) addr2);

	deallocate(addr2);
	deallocate(addr2);

	void *addr3 = allocate(17);
	printf("Allocated 17 @ %p\n", addr3);
	strcpy(addr3, "Hello, world!!");
	printf("Allocation 3: \"%s\"\n", (char *) addr3);

	return 0;

}
