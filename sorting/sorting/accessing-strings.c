#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int myInt;

char *localArray() {

	char str[100];
	char *str2 = (char *) malloc(10);

	if (str2 == NULL) {
		printf("no memory left!\n");
		exit(-1);
	}

	strcpy(str2, "<10chars.");

	str2[0] = 'B';

	char str1[100] = "Another string.";
	//char str2[10][100] = {"this is", "a 2d", "array"};

	strcpy(str, "New string.");

	return str2;

}

int main(int argc, char *argv[]) {

	char *mainStr;

	mainStr = localArray();

	printf("%s\n", mainStr);
	free(mainStr);

	return 0;

}
