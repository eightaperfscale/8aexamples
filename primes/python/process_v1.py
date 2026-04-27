# Example source code showing how to use processes.
#
# The source code is included as-is with no warranty.
#
import os
import math
import time
import threading
import multiprocessing
from primeslib import is_prime

# Process a range of primes
def process_primes (primeq, writeq):
    # Go until all entries in the processing queue are done
    while not primeq.empty():
        # Get the range
        start, end = primeq.get()

        # Find primes
        for num in range(start, end):
            if (is_prime(num)):
                writeq.put(num)

# Write primes to a file
def write_primes ():
    with open("process-v1.txt", "w") as out_stream:
        while True:
            num = writeq.get()
            if num is None:
                break
            out_stream.write(f"{num}\n")

if __name__ == '__main__':
    multiprocessing.freeze_support()

    # Prime ranges
    first = 0
    last = 100000

    # Workers are based on the number of logical cores
    workers = os.cpu_count()

    # Create queues for IPC 
    primeq = multiprocessing.Queue(maxsize=workers)
    writeq = multiprocessing.Queue(maxsize=workers * 1000)

    # Seed the right number of number ranges for each worker
    start = first
    slice = (last - first) // workers
    processes = []
    for i in range(workers):
        primeq.put((start, start + slice))
        start += slice

    # Start the write thread
    prime_thread = threading.Thread(target=write_primes)
    prime_thread.start()

    # Start the workers
    for i in range(workers):
        process = multiprocessing.Process(target=process_primes, args=(primeq, writeq))
        process.start()
        processes.append(process)

    # Wait for workers to complete
    for process in processes:
        process.join()

    # Ask the writer thread to stop
    writeq.put(None)
    prime_thread.join()

