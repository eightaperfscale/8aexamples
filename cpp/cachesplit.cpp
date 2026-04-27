// Example source code showing a cache line split.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#else  // _WIN32
#include <stdio.h>
#include <stdalign.h>
#endif // _WIN32

#ifdef _WIN32
#pragma pack(push, 1)
#else  // _WIN32
#pragma pack(1)
#endif // _WIN32
//	No natural alignment in the structure
typedef struct someData
{
	alignas(64)
	char	filler [58];
	double	dbl;
	float	flt;
} someData_t;
#ifdef _WIN32
#pragma pack(pop)
#else  // _WIN32
#pragma pop()
#endif // _WIN32

someData_t myData;

int main (void)
{
	myData.dbl = 10.5;
	return 0;
}
