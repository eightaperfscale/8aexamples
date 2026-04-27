# Example source code showing how to use processes.
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

    # Workers are based on the number of logical cores
    workers = os.cpu_count()
    slice = (last - first) // workers

    # Build a list of arguments that contain smaller number ranges
    # that we can pass to the workers
    args = []
    for i in range(0, last, slice):
        args.append((i, i + slice))

    # Use the ProcessPoolExecutor() to manage workers
    with concurrent.futures.ProcessPoolExecutor() as executor:
            # Start each worker
            tasks = [executor.submit(prime_range, a [0], a [1]) for a in args]

            # We append to the file, so remove any original
            outfile = "process-v2.txt"
            if os.path.exists(outfile):
                os.remove(outfile)

            # Process results as they are completed
            for task in concurrent.futures.as_completed(tasks):
                # Save the data
                with open(outfile, "a") as out_stream:
                    for num in task.result():
                        out_stream.write(f"{num}\n")

