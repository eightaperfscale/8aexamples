// This source code demonstrates the CPU Sets affinity API.
//
// The source code is included as-is with no warranty.
//
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

#include "isprime.h"

// Pointer conversion
#define TO_CPUSET_PTR(p) (PSYSTEM_CPU_SET_INFORMATION) p

int main (void)
{
    // Get a count of all cores, in all Processor Groups
	auto allCores = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);

    // Seed random based on time
    std::srand(unsigned(std::time(0)));

    // Randomly pick a core
    auto randCore = std::rand() % allCores;
    std::cout << "There are " << allCores << " core(s) on the platform. Selecting " << randCore << std::endl;

    // Find all CPU Sets on the platform (will fail)
	ULONG CPUSetLen;
	GetSystemCpuSetInformation(nullptr, 0, &CPUSetLen, nullptr, 0);

	// Correct failure?
	auto err = GetLastError();
	if (err != ERROR_INSUFFICIENT_BUFFER)
	{
		std::cerr << "Unable to calculate CPU Set buffer size, error " << err << std::endl;
		return 1;
	}

    // Allocate the CPU Set buffer
	std::vector<uint8_t> CPUSets(CPUSetLen);

    // Get the CPU Set information
    if (!GetSystemCpuSetInformation(TO_CPUSET_PTR(CPUSets.data()), CPUSetLen, &CPUSetLen, nullptr, 0))
    {
	    err = GetLastError();
	    std::cerr << "Unable to locate CPU Set information, error " << err << std::endl;
	    return 1;
    }

    // Each CPU Set represents a logical core 
	std::map<int, ULONG> coreMap;

    // Iterate through the CPU Set
	auto setPtr = CPUSets.data();
	auto endPtr = setPtr + CPUSetLen;
	int logicalCoreNum = 0;
    while (setPtr < endPtr)
    {
	    auto cpuInfo = TO_CPUSET_PTR(setPtr);

	    if (cpuInfo->CpuSet.Allocated == 0)
	    {
		    // Track the logical core number to 
		    coreMap [logicalCoreNum] = cpuInfo->CpuSet.Id;
	    }

	    // Next logical core
	    logicalCoreNum ++;

	    // Next entry
	    setPtr += cpuInfo->Size;
    }

    // Get the CPU Set Id for the random logical core
    std::map<int, ULONG>::iterator coreIt = coreMap.find(randCore);
    if (coreIt == coreMap.end())
    {
	    std::cerr << "Could not find logical core " << randCore << " in the CPU Set mapping.";
	    return 1;
    }

    // Set affinity for this CPU Set
    std::vector<ULONG> CPUSetIds;
    CPUSetIds.push_back(coreIt->second);
    if (!SetProcessDefaultCpuSets(GetCurrentProcess(), CPUSetIds.data(), CPUSetIds.size()))
    {
	    // Can still run if affinity can't be set
	    auto err = GetLastError();
	    std::cout << "Could not set CPU Set affinity due to error " << err << ". Continuing..." << std::endl;
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
