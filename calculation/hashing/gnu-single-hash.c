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
#include <string.h>

struct student {
	int id;
	char *name;
	struct student *next;
};

#define MAX	10000

char *randomName() {

	char *first, *last, *name;

	first = (char *) malloc(100);
	last = (char *) malloc(100);
	name = (char *) malloc(201);

	int i;
	long f, l;
	f = random();
	l = random();

	sprintf(first, "%ld", f);
	sprintf(last, "%ld", l);

	first[0] = 'A' + random() % 26;
	last[0] = 'A' + random() % 26;

	for (i = 1; i < strlen(first); i++) {
		first[i] = 'a' + (random() % 26);
	}

	for (i = 1; i < strlen(last); i++) {
		last[i] = 'a' + (random() % 26);
	}

	sprintf(name, "%s %s", first, last);

	return name;
}

void init() {
	assert(hcreate(MAX));
}

void insert(struct student *s) {
	ENTRY item;
	item.key = malloc(256);
	assert(item.key); // DO NOT DO THIS: handle errors appropriately
	item.data = s->name;
	sprintf(item.key, "%d", s->id);

	ENTRY *retval;
	assert(hsearch(item, ENTER));
}

ENTRY *search(int id) {
	ENTRY item;
	item.key = malloc(256);
	assert(item.key);
	sprintf(item.key, "%d", id);

	return hsearch(item, FIND);

}

void print_student(ENTRY *s) {
	if (!s)
		printf("NULL\n");
	else
		printf("%s %s\n", s->key, (char *) s->data);
}

int main() {
	int i;

	struct student * S;

	init();

	for (i = 0; i < MAX; i++) {

		S = (struct student *) malloc(sizeof(struct student));
		S->id = i;
		S->name = randomName();
		// printf("Inserting: %d %s\n", S->id, S->name);

		insert(S);
	}

	print_student(search(1111));
	print_student(search(1230));

	hdestroy();

}
