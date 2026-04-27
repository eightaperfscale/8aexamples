# Example source code to find prime numbers with an improved algorithm.
#
# The source code is included as-is with no warranty.
#
import math

# Check for primes
def is_prime (num):
    # 0 and 1 cannot be primes
    if num <= 1:
        return False
		
    # 2 is a prime
    if num == 2:
        return True
		
    # Check the rest of the number range
    limit = int(math.sqrt(num))
    for i in range(2, limit + 1):
        if num % i == 0:
            return False

    return True

first = 0
last = 100000

with open("primes-v2.txt", "w") as out_stream:
	for i in range(first, last):
			if (is_prime(i)):
				out_stream.write(f"{i}\n")

