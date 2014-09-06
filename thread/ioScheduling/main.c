#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int main(void) {
	char filename[20] = "number";
	int nFiles = 10;
	int nIterations = 100;
	int i;

	for (i = 0; i < nFiles; i++) {
		FILE *pFile;
		sprintf(filename + 6, "%d", i);
		strcat(filename, ".txt");
		pFile = fopen(filename, "w+");
		printf("%s\n", filename);
		int n;
		for (n = 0; n < nIterations; n++) {
			fputs("test \t", pFile);
		}
		fclose(pFile);
		filename[6] = '\0';
	}

	for (i = 0; i < nFiles; i++) {
		FILE *pFile;
		sprintf(filename + 6, "%d", i);
		strcat(filename, ".txt");
		pFile = fopen(filename, "r");
		char str[100];
		while (fgets(str, sizeof(str), pFile) != NULL) {
			printf("%s", str);
		}
	}

	return 0;
}
