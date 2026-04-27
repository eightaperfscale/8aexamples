// This source code demonstrates the Classic affinity API.
//
// The source code is included as-is with no warranty.
//
#include <Windows.h>
#include <iostream>
#include <cstdlib>

#include "isprime.h"

int main (void)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto allCores = sysinfo.dwNumberOfProcessors;

    // Seed random based on time
    std::srand(unsigned(std::time(0)));

    // Randomly pick a core
    auto randCore = std::rand() % allCores;
    std::cout << "There are " << allCores << " core(s) on the platform. Selecting " << randCore << std::endl;

     // Classic affinity uses a bitmap
     if (!SetProcessAffinityMask(GetCurrentProcess(), 1ULL << randCore))
     {
         // Can still run if affinity can't be set
         auto err = GetLastError();
         std::cout << "Could not set classic affinity due to error " << err << ". Continuing..." << std::endl;
     }

    // Loop for a while to show the core is active
	auto result = 0;
	for (auto i = 0; i < 100000000; i++)
	{
		if (isPrime (i))
		{
			result++;
		}
	}

	std::cout << "Primes found " << result << std::endl;

	return 0;
}

