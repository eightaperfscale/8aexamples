# Example source code using a reusable prime numbers algorithm. This file
# is included in a number of other examples.
#
# The source code is included as-is with no warranty.
#
import math

# Check for primes
def is_prime (num):
    # Split the input processing into distinct 
    # number ranges
    if (num > 3):
        # The mod 6 algorithm only works with numbers over 3
        rem = num % 6

        # We are only interested in remainders 1 and 5
        # which relate to 6k + 1 (remainder 1) and 
        # 6k – 1 (remainder 5)
        if rem == 1 or rem == 5:
            limit = int(math.sqrt(num))
            for i in range(5, limit + 1, 6):
                if num % i == 0 or num % (i + 2) == 0:
                    return False
        else:
            return False
    else:
       # The number has to be 0, 1, 2 or 3. We know
       # 0 and 1 cannot be primes
       if num <= 1:
           return False

    # Number is a prime
    return True

