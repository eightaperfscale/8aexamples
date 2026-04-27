// Example source code to show processing data backwards after prefetch
// of cache lines.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#define ssize_t SSIZE_T
#endif // _WIN32

#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cctype>

constexpr size_t CLSize = 64;
constexpr size_t maxLines = 8;
constexpr size_t batchSize = maxLines * CLSize;
constexpr size_t bufferSize = 1024;

void ProcessData (char *data, size_t len)
{
	// Loop while there is enough data
	while (len > batchSize)
	{
		// Load the next 8 cache lines
		for (auto i = 0; i < maxLines; i ++)
		{
			// Only needs 1 byte to be loaded to 
			// get the entire cache line
			char d = data [i * CLSize];
		}

		// Process backwards
		for (ssize_t i = batchSize - 1; i >= 0; i --)
		{
			// Convert to uppercase
			data [i] = std::toupper(data [i]);
		}

		// Move to the next batch
		data += batchSize;
		len -= batchSize;
	}

	// Process last part of the data
	for (ssize_t i = len - 1; i >= 0; i --)
	{
		data [i] = std::toupper(data [i]);
		len --;
	}
}

int main (void)
{
	// Fill a 1024 character buffer with lower case letters
    char buffer [bufferSize + 1] = {0};
	for (auto i = 0; i < bufferSize; i ++)
	{
		buffer [i] = 'a' + (i % 26);
	}

	// Output progress
	std::cout << "The initial buffer is " << buffer << std::endl;

	// Convert to upper case (backwards)
	ProcessData(buffer, bufferSize);
	
	// Output progress
	std::cout << "The buffer is now " << buffer << std::endl;

	return 0;
}



