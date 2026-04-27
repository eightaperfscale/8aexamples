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
            # Add the prime into our queue
            q.put(num)

# Prime number range
first = 0
last = 100000

# Create a result queue
prime_q = queue.Queue()

# Number of worker threads is the number of logical cores
workers = os.cpu_count()
slice = last // workers
start = first
threads = []
for i in range(0, workers):
    # Create and start the thread
    thread = threading.Thread(target=prime_range, args=(prime_q, start, start + slice))
    thread.start()

    # Track the thread and move to the next processing slice
    threads.append(thread)
    start += slice

# Wait for threads to complete
for thread in threads:
    thread.join()

# Output the results
with open("thread-v1.txt", "w") as out_stream:
     while not prime_q.empty():
         out_stream.write(f"{prime_q.get()}\n")

