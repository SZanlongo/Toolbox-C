#include <stdio.h>
#include <stdlib.h>

#define MASK (1U<<31)

void print_bits(unsigned int x) {

	int i;

	for (i = 0; i < sizeof(x) * 8; i++) {

		if (x & MASK)
			printf("1");
		else
			printf("0");

		if ((i + 1) % 8 == 0 && i != 31)
			printf(" ");

		x = x << 1;
	}

	printf("\n");

}

void set_bit(unsigned int *x, int pos) {

	// position starts from one

	if (pos > 32) {
		printf("setting invalid bit.\n");
		exit(1);
	}

	*x = (*x) | (1 << (pos - 1));

}

void reset_bit(unsigned int *x, int pos) {

	if (pos > 32) {
		printf("setting invalid bit.\n");
		exit(1);
	}

	*x = (*x) & ~(1 << (pos - 1));

}

int get_bit(unsigned int x, int pos) {

	if (pos > 32) {
		printf("setting invalid bit.\n");
		exit(1);
	}

	if (x & (1 << (pos - 1)))
		return 1;
	else
		return 0;

}

void toggle_bit(unsigned int *x, int pos) {

	if (pos > 32) {
		printf("setting invalid bit.\n");
		exit(1);
	}

	if (get_bit(*x, pos))
		reset_bit(x, pos);
	else
		set_bit(x, pos);

}

int find_zero_bit(unsigned int x) {

	// returns a valid position if found
	// else returns -1;
	int pos = 1;
	while (pos <= 32) {
		if (get_bit(x, pos) == 0) {
			return pos;
		} else {
			pos++;
		}
	}

	return -1;
}

