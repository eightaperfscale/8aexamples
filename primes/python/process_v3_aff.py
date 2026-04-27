# Example source code showing how to use affinity with processes.
#
# The source code is included as-is with no warranty.
#
import os
import psutil
import multiprocessing
import concurrent.futures
from affinity import platform_affinity
from primeslib import is_prime 

# Locate a series prime numbers
def prime_range (core, first, last):
    p = psutil.Process(os.getpid())
    p.cpu_affinity([core])

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

    # Get the number of cores we can use
    plat_aff = platform_affinity()
    cores = plat_aff.find_best_cores()
    workers = len(cores)

    # Create number ranges for each worker
    args = []
    start = first
    slice = (last - first) // workers
    for i in range(0, workers - 1):
       args.append((cores [i], start, start + slice))
       start += slice

    # Last range might be slightly different
    args.append((cores [workers - 1], start, last))

    with concurrent.futures.ProcessPoolExecutor() as executor:
            # Process results in order of submission
            results = executor.map(prime_range, *zip(*args))

            # We append to the file, so remove any original
            outfile = "process-v3-aff.txt"
            if os.path.exists(outfile):
                os.remove(outfile)

            for result in results:
                with open(outfile, "a") as out_stream:
                    for num in result:
                        out_stream.write(f"{num}\n")

