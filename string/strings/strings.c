#include <stdio.h>
#include <string.h>

#include "reverse.h"

int main(int argc, char *argv[]) {

	char str[2][100];

	strcpy(str[0], "This is string 1.");
	strcpy(str[1], "This is string 2.");

	printf("Length=%d String=\"%s\"\n", (int) strlen(str[0]), str[0]);

	reverse(str);

	printf("\n");

	return 0;

}
