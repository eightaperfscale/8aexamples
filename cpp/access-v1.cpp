// Example source code to show atomic access.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <thread>
#include <vector>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

uint64_t counter = 0;

void IncCounter (void)
{
	counter ++;
}

int main (void)
{
	// Workers are based on the number of logical cores
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto workers = sysinfo.dwNumberOfProcessors;
#else  // _WIN32
	auto workers = sysconf(_SC_NPROCESSORS_ONLN);
#endif // _WIN32

	// Create workers
	std::vector<std::thread> workerThreads;
	for (auto i = 0; i < workers; i ++)
	{
		// Create and track the worker thread
		workerThreads.emplace_back(IncCounter);
	}

	// Wait for all workers to complete
	for (auto &t : workerThreads)
	{
		if (t.joinable()) 
		{
			t.join();
		}
	}

	// Output result
	std::cout << "The count is " << counter << std::endl;

	return 0;
}

