# Example source code to show how coroutines can be used.
#
# The source code is included as-is with no warranty.
#
import os
import queue
import asyncio
from primeslib import is_prime

# Process a small series of numbers
async def prime_range (q, first, last):
    for num in range(first, last):
        if is_prime(num):
            # Add the prime our queue
            q.put(num)
    return True

async def get_primes():
    # Prime number range
    first = 0
    last = 100000

    # Create the result queue
    prime_q = queue.Queue()

    # Number of worker threads is the number of logical cores
    workers = os.cpu_count()
    slice = last // workers
    start = first

    # Create the arguments for the coroutine
    args = []
    for i in range(0, workers):
        
        args.append((prime_q, int(start), int(start + slice)))
        start += slice

    # Create a list of coroutines based on the arguments
    coroutines = [prime_range(q, start, end) for q, start, end in args]

    # Run the coroutines
    results = await asyncio.gather(*coroutines)

    # Output the results
    with open("coroutine-v1.txt", "w") as out_stream:
         while not prime_q.empty():
             out_stream.write(f"{prime_q.get()}\n")

if __name__ == "__main__":
    asyncio.run(get_primes())

