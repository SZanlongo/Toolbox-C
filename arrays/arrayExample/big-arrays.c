#include <stdio.h>
#include <stdlib.h>

int i;

int main(int argc, char *argv[]) {

	int A[5000];
	int *B;
	B = A;
	char * str1 = (char *) malloc(17);
	char * str2 = (char *) malloc(17);
	char * str3 = (char *) malloc(17);

	//heap allocations
	for (i = 0; i < 17; i++) {
		str1[i] = 'A' + i;
		str2[i] = 'A' + i;
		str3[i] = 'A' + i;
	}
	str1[i] = '\0';
	str2[i] = '\0';
	str3[i] = '\0';

	printf("size of A: %zu\n", sizeof(A) / sizeof(A[0]));
	printf("size of B: %zu\n", sizeof(B) / sizeof(B[0]));

	printf("STR1: [%p] %s\nSTR2: [%p] %s\nSTR3: [%p] %s\n", str1, str1, str2,
			str2, str3, str3);

	// array can be accessed beyond bounds
	for (i = 0; i < 5001; i++) {
		printf("%d: %p\n", i, &A[i]);
		A[i] = 0;
	}

	return 0;

}
