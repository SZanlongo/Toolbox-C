#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	char * name[] = { "Illegal month", "Jan", "Feb", "March" };

	char *str = (char *) malloc(10);
	strcpy(str, "hello!");

	char aname[][15] = { "Illegal month", "Jan", "Feb", "March" };

	// should be an address in the data section (unmodifiable part)
	printf("Address of name[0] = %p\n", name[0]);
	name[0] = "Another illegal month";

	// again a data section address
	printf("Address of name[0] = %p\n", name[0]);

	// address of a stack location
	name[0] = (char *) 0x7fff5fbff410;
	printf("Address of name[0] = %p\n", name[0]);

	// address of a heap location
	name[0] = str;
	printf("Address of name[0] = %p\n", name[0]);

	printf("aname = %s\n", aname[0]);

	aname[0][0] = 'i';

	printf("modified aname = %s\n", aname[0]);

	// this should be an address in the stack
	printf("Address of aname[0] = %p\n", aname[0]);

	// This doesn't work because we
	// are modifying a constant string
	name[0][0] = 'i';

	return 0;

}
