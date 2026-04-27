// Example source code showing variables aligned to a 64 byte cache line.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <stdalign.h>

alignas(64) int x;
alignas(64) int y;

int main (void)
{
	x = 10;
	y = 20;
	printf("X * Y == %d\n", x * y);
	return 0;
}

