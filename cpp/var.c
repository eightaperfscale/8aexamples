// This example demonstrates two variables sharing the same cache line.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>

int x;
int y;

int main (void)
{
	x = 10;
	y = 20;
	printf("X * Y == %d\n", x * y);
	return 0;
}

