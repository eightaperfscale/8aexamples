// Example source code implementation of the prime mod 6 algorithm.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

// Determine if a number is a prime
inline bool isPrime (const uint64_t num)
{
	if (num <= 4)
	{
		if (num == 2 || num == 3)
		{
			return true;
		}

		return false;
	}

    // Only interested in remainder 1 and 5
    const uint64_t rem = num % 6;
    if (rem == 1 || rem == 5)
    {
        const uint64_t limit = sqrt(num);
        for (uint64_t i = 5; i <= limit; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0)
            {
                // Not a prime
                return false;
            }
        }

        // Number is a prime
        return true;
    }

    // Not a prime
    return false;
}
