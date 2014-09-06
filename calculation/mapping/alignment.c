#include <stdio.h>

typedef struct {

	char c;
	int i;

} charint;

int main(int argc, char *argv[]) {

	char c;
	short int s;
	char d, e, f;
	int i;
	charint ci;

	printf("charsize = %zu\n", sizeof(c));
	printf("shortintsize = %zu\n", sizeof(s));
	printf("intsize = %zu\n", sizeof(i));
	printf("charintsize = %zu\n", sizeof(ci));

	printf("charint(c) = %p, charint(i) = %p\n", &(ci.c), &(ci.i));

	*(&(ci.c) + 4 * sizeof(char)) = 10;

	printf("ci.i = %d\n", ci.i);

	return 0;

}
