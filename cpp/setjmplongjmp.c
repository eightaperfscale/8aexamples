// Example code to demonstrate setjmp() and longjmp() capabilty.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <setjmp.h>

jmp_buf jmpBuf;

void FuncC (int count)
{
	printf("%s: call %d\n", __FUNCTION__, count);
	if (count % 10 == 0)
	{
		longjmp(jmpBuf, 1);
	}
}

void FuncB (void)
{
	static int accessed = 0;
	FuncC(++ accessed);
}

void FuncA (void)
{
	int rc = setjmp(jmpBuf);
	if (rc == 0)
	{
		for (int i = 0; i < BUFSIZ; i ++)
		{
			FuncB();
		}
	}
	else
	{
		printf("longjmp() fired!\n");
	}
}

int main (void)
{
	FuncA();
}

