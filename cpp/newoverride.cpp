// Example source code showing how to override the C++ new operator to 
// allocate classes on a cache line boundary.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <new>
#include <cstdlib>

// Help portability
#ifdef _WIN32
#define ALLOC_ALIGNED(l,a)	_aligned_malloc(l, a)
#define FREE_ALIGNED(p)		_aligned_free(p)
#else  // _WIN32	
#define ALLOC_ALIGNED(l,a)	aligned_alloc(a, l)
#define FREE_ALIGNED(p)		free(p)
#endif // _WIN32	

class someClass
{
	public:
	int x;

	// Aligned new operator
	void * operator new (size_t size)
	{
		// Round up to the nearest 64
		size_t adjusted = (size + 63) & ~63;

		// Allocate aligned
		auto p = ALLOC_ALIGNED(adjusted, 64);

		if (p == nullptr)
		{
			// Error
			throw std::bad_alloc();
		}

		// All okay
		return p;
	}

	// Delete operator
	void operator delete (void* ptr) noexcept 
	{
     		FREE_ALIGNED(ptr);
	}
};

int main (void)
{
	// Create an object
	auto myClass = new someClass();

	// Access the class
	myClass->x = 10;

	// Done with the class
	delete(myClass);
	return 0;
}

