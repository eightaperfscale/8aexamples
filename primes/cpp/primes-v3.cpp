// Example source code using the prime number mod 6 algorithm.
//
// The source code is included as-is with no warranty.
//
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

#include "isprime.h"

// Number range
const uint64_t first    = 0;
const uint64_t last     = 100000;

// Ouput filename
const char *outFile     = "primes-v3.txt";

int main (void)
{
    FILE *outStream = fopen(outFile, "w");
    if (!outStream)

    {
        fprintf(stderr, "Could not open %s error %d\n", outFile, errno);
        return 1;
    }

    // Deal with the full number range
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
