#include <stdio.h>
#include <string.h>

int myInt;

char *localArray() {

	char str[100];
	strcpy(str, "New string.");

	return str;

}

int main(int argc, char *argv[]) {

	char *mainStr;

	mainStr = localArray();

	printf("%s\n", localArray());

	printf("%s\n", mainStr);

	return 0;

}
