#include <stdio.h>
#include <string.h>

int reverse(char str[][100]) {

	char *index;
	int len, N;

	N = len = strlen(str[0]);
	index = str[0] + strlen(str[0]) - 1;

	while (N > 0) {
		printf("%c", *index);
		index--;
		N--;
	}

	return len;

}
