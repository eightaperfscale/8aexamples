// Example source code showing a SIMD memory copy.
//
// The source code is included as-is with no warranty.
//
#include <iostream>
#include <immintrin.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>

#define DEBUG 0
#if (DEBUG == 1)
#define DEBUGOUT(s)	std::cout << s << std::endl;
#else	// DEBUG
#define DEBUGOUT(s)
#endif	// DEBUG

inline void ByteCopy (char *dest, char *source, const size_t size)
{
	DEBUGOUT("ByteCopy");

	// Copy byte by byte
	for (auto i = 0; i < size; i ++)
	{
		*dest ++ = *source ++;
	}
}

void SIMDMemcpy (void *dest, void *source, const size_t size)
{
	// Size is in bytes
	if (size < sizeof(__m128i))
	{
		// Smaller than a SIMD variable - have to copy byte by byte
		ByteCopy((char *) dest, (char *) source, size);
		return;
	}

	// SIMD size or greater data - create SIMD pointers
	__m128i *SIMDDest = (__m128i *) dest;
	__m128i *SIMDSource = (__m128i *) source;

	//	Data remaining
	auto remaining = size;

	// Is the data aligned?
	if ((uint64_t(dest) & 0xF) == 0 && (uint64_t(source) & 0xF) == 0)
	{
		// Aligned copy
		DEBUGOUT("Aligned");
		while (remaining > 0 && remaining >= sizeof(__m128i))
		{
			// Copy data
			__m128i s = _mm_load_si128(SIMDSource);
			_mm_store_si128(SIMDDest, s);

			// Adjust pointers
			SIMDDest ++;
			SIMDSource ++;
			
			// Adjust remaining
			remaining -= sizeof(__m128i);
		}
	}
	else
	{
		// Unaligned copy
		DEBUGOUT("Unaligned");
		while (remaining > 0 && remaining >= sizeof(__m128i))
		{
			// Copy data
			__m128i s = _mm_loadu_si128(SIMDSource);
			_mm_storeu_si128(SIMDDest, s);

			// Adjust pointers
			SIMDDest ++;
			SIMDSource ++;
			
			// Adjust remaining
			remaining -= sizeof(__m128i);
		}
	}

	// Copy any trailing bytes
	if (remaining > 0)
	{
		ByteCopy((char *) SIMDDest, (char *) SIMDSource, remaining);
	}
}

int main (void)
{
	// Seed random based on time
	std::srand(unsigned(std::time(0)));

	// Create a source vector and populate with random data
	std::vector<uint64_t> source;
	for (auto i = 0; i < 1024; i ++)
	{
		uint64_t v = std::rand() % (i + 1);
		source.push_back(v);
	}

	// Create the destination vector
	std::vector<uint64_t> dest(source.size());

	// Copy the data
	SIMDMemcpy(dest.data(), source.data(), source.size() * sizeof(uint64_t));

	// Validate the copy
	for (auto i = 0; i < source.size(); i ++)
	{
		if (dest.at(i) != source.at(i))
		{
			std::cerr << "Value at offset " << i << " did not copy" << std::endl;
			break;
		}
	}

	return 0;
}

