/* Usage of the glibc hash table (hcreate_r, hsearch_r).
 TODO: free all dynamically allocated memory. */

#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE

// Defined to get the declaration of hsearch_data. Check this code for details.
// http://elephant.cs.fiu.edu/source/xref/glibc-2.11.2/misc/search.h#96
#define __USE_GNU

#include <search.h>
#include <assert.h>

struct student {
	int id;
	char name[256];
	struct student *next;
};

// two hash tables
struct hsearch_data htab1;
struct hsearch_data htab2;

#define MAX	10000

struct student data[] = { { 111, "koller", NULL }, { 222, "andrade", NULL }, {
		333, "rangaswami", NULL } };
int count = 3;

void init() {
	assert(hcreate_r(MAX, &htab1));
	assert(hcreate_r(MAX, &htab2));
}

void insert(struct hsearch_data *htab, struct student *s) {
	ENTRY item;
	item.key = malloc(256);
	assert(item.key); // DO NOT DO THIS: handle errors appropriately
	item.data = s->name;
	sprintf(item.key, "%d", s->id);

	ENTRY *retval;
	assert(hsearch_r(item, ENTER, &retval, htab));
}

ENTRY *search(struct hsearch_data *htab, int id) {
	ENTRY item;
	item.key = malloc(256);
	assert(item.key);
	sprintf(item.key, "%d", id);

	ENTRY *retval;
	hsearch_r(item, FIND, &retval, htab);

	return retval;
}

void print_student(ENTRY *s) {
	if (!s)
		printf("NULL\n");
	else
		printf("%s %s\n", s->key, (char *) s->data);
}

int main() {
	int i;

	init();

	for (i = 0; i < count; i++) {
		printf("%d\n", data[i].id);
		insert(&htab1, &data[i]);
	}

	print_student(search(&htab1, 111));
	print_student(search(&htab2, 123));
}
