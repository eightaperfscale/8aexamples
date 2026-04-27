# Example source code for use with external affinity tools like taskset (Linux)
# or start /affinity (Windows). The example shows how affinity affects the
# primes algorithm without writing any Python based affinity code.
#
# The source code is included as-is with no warranty.
#
import os
import multiprocessing
import concurrent.futures
from primeslib import is_prime 

# Locate a series prime numbers
def prime_range (first, last):
    primes = []
    for num in range(first, last):
        if is_prime(num):
            primes.append(num)
    return primes

if __name__ == '__main__':
    multiprocessing.freeze_support()

    # Number range
    first = 0
    last = 100000

    # Only 1 worker
    workers = 1
    slice = last // workers
    args = []
    for i in range(0, last, slice):
        args.append((i, i + slice))

    with concurrent.futures.ProcessPoolExecutor() as executor:
            # Process results in order of submission
            results = executor.map(prime_range, *zip(*args))

            # We append to the file, so remove any original
            outfile = "affinity-v1.txt"
            if os.path.exists(outfile):
                os.remove(outfile)

            for result in results:
                with open(outfile, "a") as out_stream:
                    for num in result:
                        out_stream.write(f"{num}\n")

