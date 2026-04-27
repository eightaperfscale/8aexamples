# Example source code to find prime numbers.
#
# The source code is included as-is with no warranty.
#

# Check for primes
def is_prime (num):
    # 0 and 1 cannot be primes
    if num <= 1:
        return False
		
    # 2 is prime
    if num == 2:
        return True
		
    # Check the rest of the number range
    for i in range(2, num):
        if num % i == 0:
            return False

    return True

first = 0
last = 100000

with open("primes-v1.txt", "w") as out_stream:
	for i in range(first, last):
			if (is_prime(i)):
				out_stream.write(f"{i}\n")

