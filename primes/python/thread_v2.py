# Example source code showing how to use threads.
#
# The source code is included as-is with no warranty.
#
import os
import queue
import threading
from primeslib import is_prime

# Process a small series of numbers
def prime_range (q, first, last):
    for num in range(first, last):
        if is_prime(num):
            # Add the prime our queue
            q.put(num)

# Prime number range
first = 0
last = 100000

# Number of worker threads is the number of logical cores
workers = os.cpu_count()

# Create a result queue for each worker
queues = []
for i in range(0, workers):
    queues.append(queue.Queue())

# Create the number range each worker will process
slice = last // workers
start = first
threads = []
for q in queues:
    # Create and start the thread
    thread = threading.Thread(target=prime_range, args=(q, start, start + slice))
    thread.start()

    # Track the thread
    threads.append(thread)

    # Next processing slice
    start += slice

# Wait for threads to complete
for thread in threads:
    thread.join()

# Output the results
with open("thread-v2.txt", "w") as out_stream:
     for q in queues:
         while not q.empty():
             out_stream.write(f"{q.get()}\n")

