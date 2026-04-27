// Example source code showing the prime number mod 6 algorithm using
// AMD/Intel SIMD instructions.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <immintrin.h>

// Number range
const uint64_t first    = 0;
const uint64_t last     = 100000;

// Ouput filename
const char *outFile     = "primes-simd.txt";

// Determine if a number is a prime
inline bool isPrime (const uint64_t num)
{
	//	Deal with small primes <= 3
	if (num <= 4)
	{
		if (num == 2 || num == 3)
		{
			return true;
		}

		return false;
	}

	uint64_t rem = num % 6;
	
	if (rem == 1 || rem == 5)
	{
		uint64_t ourSQ = sqrt(num);
		for (uint64_t i = 5; i <= ourSQ; i += 6)
		{
			// Convert the current number to floating point
			__m128d valHi = _mm_cvtsi64_sd(_mm_setzero_pd(), num);
   			__m128d valLo = _mm_cvtsi64_sd(_mm_setzero_pd(), num);
			__m128d SIMDNum = _mm_unpacklo_pd(valHi, valLo);

			// Convert the test number to floating point
			valHi = _mm_cvtsi64_sd(_mm_setzero_pd(), i);
   			valLo = _mm_cvtsi64_sd(_mm_setzero_pd(), i + 2);
			__m128d SIMDDiv = _mm_unpacklo_pd(valHi, valLo);

			// Produce a mod result
			__m128d q = _mm_div_pd(SIMDNum, SIMDDiv);
			__m128d t = _mm_floor_pd(q);
			__m128d m = _mm_mul_pd(t, SIMDDiv);
			__m128d r = _mm_sub_pd(SIMDNum, m);

			// Convert to SIMD integer
			__m128i SIMDInt = _mm_castpd_si128(r);

			// Extract high and low values
			uint64_t lo = _mm_extract_epi64(SIMDInt, 0);
			uint64_t hi = _mm_extract_epi64(SIMDInt, 1);

			if (hi == 0 || lo == 0)
			{
				// Not prime
				return false;
			}
		}

		// Prime number
		return true;
	}

	// Not prime
	return false;
}

int main (void)
{
    FILE *outStream = fopen(outFile, "w");
    if (!outStream)

    {
        fprintf(stderr, "Could not open %s error %d\n", outFile, errno);
        return 1;
    }

    // Process the number range
    for (uint64_t num = first; num <= last; num ++)
    {
        if (isPrime(num))
        {
            fprintf(outStream, "%lu\n", num);
        }
    }

    // Close the file
    fclose(outStream);
    return 0;
}
