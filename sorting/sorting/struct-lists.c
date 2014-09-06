#include <stdio.h>
#include <stdlib.h>

// Conventional struct definition
struct TEST {
	int A;
	char B;
	float C;
};

struct TEST T;

// Useful struct definition
typedef struct __node {
	char str[100];
	struct __node *next;
} node;

int main(int argc, char *argv[]) {

	node *strings, *iterator;

	iterator = strings = (node *) malloc(sizeof(node));
	iterator->next = NULL;

	int ns = 0;
	FILE *fp = fopen("testfile", "r");

	while (fgets(iterator->str, 99, fp) != NULL) {
		iterator->next = (node *) malloc(sizeof(node));
		iterator = iterator->next;
		iterator->next = NULL;
	}

	iterator = strings;
	while (iterator) {
		printf("[%d]: %s", ns++, iterator->str);
		iterator = iterator->next;
	}

	return 0;

}
