// Example source code showing variables aligned to the CL_SIZE define which 
// can be passed in from a command line option like -DCL_SIZE=xx (Linux) or
// /DCL_SIZE=xx (Windows)
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <stdalign.h>

alignas(CL_SIZE) int x;
alignas(CL_SIZE) int y;

int main (void)
{
	x = 10;
	y = 20;
	printf("X * Y == %d\n", x * y);
	return 0;
}

