// Source code example using a C++ placement new to allocate a class aligned
// on a cache line boundary.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <new>
#include <cstdlib>

// Class definition
class someClass
{
	public:
		int x;
};

// Help portability
#ifdef _WIN32
#define ALLOC_ALIGNED(l,a)	_aligned_malloc(l, a)
#define FREE_ALIGNED(p)		_aligned_free(p)
#else  // _WIN32	
#define ALLOC_ALIGNED(l,a)	aligned_alloc(a, l)
#define FREE_ALIGNED(p)		free(p)
#endif // _WIN32	

int main (void)
{
	// Align the buffer
	char *rawBuffer = (char *) ALLOC_ALIGNED(1024, 64);
	
	// Create an object
	auto myClass = new (rawBuffer) someClass();

	// Access the class
	myClass->x = 10;

	// Other code goes here

	// Done with the buffer
	FREE_ALIGNED(rawBuffer);

	return 0;
}
	

