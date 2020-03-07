#include "stdio.h"

int
main()
{
	int n = 1;

	// little endian if true
	if (*(char *)&n == 1)
		fprintf(stderr, "little endian\n");
	else
		fprintf(stderr, "big endian\n");
}
