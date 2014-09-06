#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct __person {
	char first[20];
	char last[20];
	int age;
} person;

int main(int argc, char *argv[]) {

	person P[8];
	int i;

	FILE * ifp = fopen("infile", "r");
	if (!ifp) {
		fprintf(stderr, "[%d]: ", errno);
		perror("Could not open input file");
		exit(1);
	}

	FILE * ofp = fopen("outfile", "w+");
	if (ofp == NULL) {
		perror("Could not open output file");
		//exit(1);
		return 0;
	}

	for (i = 0; i < 4; i++) {
		fscanf(ifp, "%s %s %d\n", P[i].first, P[i].last, &P[i].age);
	}

	fseek(ifp, 1, SEEK_SET);

	for (; i < 8; i++) {
		fscanf(ifp, "%s %s %d\n", P[i].first, P[i].last, &P[i].age);
	}

	for (i = 0; i < 8; i++) {
		fprintf(ofp, "%s %s %d\n", P[i].first, P[i].last, P[i].age);
	}

	fclose(ifp);
	fclose(ofp);

	return 0;

}
