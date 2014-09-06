#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	const int x = 1000;

	// re-assignment of consts not permitted
	//x = 200;

	const int * a1 = (int *) malloc(4 * sizeof(int));
	int * const a2 = (int *) malloc(16);
	int const * a3 = (int *) malloc(16);

	// target of a1 cannot be modified
	// a1[0] = 0;

	// can we modify a1 itself? -- YES
	a1 = 0;

	// can we modify target of a2? -- YES
	a2[0] = 0;

	// can I modify a2? -- NO
	// a2 = 0;

	// modify target of a3? -- NO
	// a3[0] = 0;

	// can we modify a3 itself? -- YES
	a3 = 0;

	return 0;

}
