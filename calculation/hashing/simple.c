/* Simplest form of hash table where the hash function is just one to one.
 By doing this we have a constant O(1) search time but with a lot of
 not used memory (for the large table). */

#include <stdio.h>

//http://users.cis.fiu.edu/~raju/WWW/
struct student {
	int id;
	char name[256];
};

#define MAX	1000

struct student data[] = { { 111, "koller" }, { 222, "andrade" }, { 333,
		"rangaswami" } };
int count = 3;

struct student *htable[MAX];

void init() {
	int i;
	for (i = 0; i < MAX; i++)
		htable[i] = NULL;
}

void insert(struct student *s) {
	int index = s->id; // simplest form of a hash table
	// the key is the index
	// hash(id) -> id

	htable[index] = s;
}

struct student *search(int id) {
	int index = id;
	return htable[index];
}

void print_student(struct student *s) {
	if (!s)
		printf("NULL\n");
	else
		printf("%d %s\n", s->id, s->name);
}

int main() {
	int i;

	init();

	for (i = 0; i < count; i++) {
		printf("%d\n", data[i].id);
		insert(&data[i]);
	}

	print_student(search(111));
	print_student(search(123));
}
