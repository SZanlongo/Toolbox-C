#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This program is incomplete
// We will finalize this in our next class.

typedef struct __student {
	char name[100];
	long ID;
	struct __student *next;
} student;

int namecmp(const void *s1, const void *s2) {

	student *S1, *S2;
	S1 = (student *) s1;
	S2 = (student *) s2;

	return S1->ID - S2->ID;

}

int main(int argc, char *argv[]) {

	int N = 0;
	student *list, *iterator, *prev;

	prev = iterator = list = (student *) malloc(sizeof(student));

	list->next = NULL;

	for (i = N; i < random() % 100 + 10; i++) {

		/*     STUPID WAY
		 *
		 *     strcpy(iterator->name,"STUDENT");
		 *     iterator->name[7] = '0' + 9 - i;
		 *     iterator->name[8] = '\0';
		 */

		// The right way!
		sprintf(iterator->name, "STUDENT%d", i);

		iterator->ID = random();

		iterator->next = (student *) malloc(sizeof(student));

		prev = iterator;
		iterator = iterator->next;
		iterator->next = NULL;

	}

	free(iterator);
	prev->next = NULL;

	printf("STUDENTS\n");

	for (i = 0; i < 10; i++) {
		printf("%ld: %s\n", S[i].ID, S[i].name);
	}

	// sort the strings alphabetically
	qsort(S, 10, sizeof(student), namecmp);

	printf("\nSORTED STRINGS\n");
	for (i = 0; i < 10; i++) {
		printf("%ld: %s\n", S[i].ID, S[i].name);
	}

	return 0;

}
