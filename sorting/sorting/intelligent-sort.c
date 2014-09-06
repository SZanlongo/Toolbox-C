#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __student {
	char name[100];
	long ID;
	struct __student *next;
} student;

int studentptrcmp(const void *sp1, const void *sp2) {

	student *S1, *S2;

	S1 = *((student **) sp1);
	S2 = *((student **) sp2);

	return S1->ID - S2->ID;

}

int main(int argc, char *argv[]) {

	int N = 0, i;
	student *list, *iterator, *prev;

	prev = iterator = list = (student *) malloc(sizeof(student));

	list->next = NULL;

	for (; N < random() % 100 + 10; N++) {

		/*     STUPID WAY
		 *
		 *     strcpy(iterator->name,"STUDENT");
		 *     iterator->name[7] = '0' + 9 - i;
		 *     iterator->name[8] = '\0';
		 */

		// The right way!
		sprintf(iterator->name, "STUDENT%d", N);

		iterator->ID = random();

		iterator->next = (student *) malloc(sizeof(student));

		prev = iterator;
		iterator = iterator->next;
		iterator->next = NULL;

	}

	free(iterator);
	prev->next = NULL;

	student * SP[N];

	iterator = list;
	for (i = 0; i < N; i++) {
		SP[i] = iterator;
		iterator = iterator->next;
	}

	printf("SORTED LIST OF STUDENTS\n");

	// sort the strings alphabetically
	qsort(SP, N, sizeof(student *), studentptrcmp);

	printf("\nSORTED STRINGS\n");
	for (i = 0; i < N; i++) {
		printf("%ld: %s\n", SP[i]->ID, SP[i]->name);
	}

	return 0;

}
