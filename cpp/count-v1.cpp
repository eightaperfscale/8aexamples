// Example source code that shows false sharing.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <atomic>
#include <thread>
#include <vector>
#include <cmath>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

// Worker processing counts
uint64_t *workerCounts;

std::atomic<bool> running = true;

// Count worker
void Counter (uint64_t *count)
{
	while (running)
		(*count) ++;
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

	// Allocate a buffer to track how much each worker processes
	workerCounts = new uint64_t [workers]();

	// Create workers
	std::vector<std::thread> workerThreads;
	for (auto i = 0; i < workers; i ++)
	{
		//	Create and track the worker thread
		workerThreads.emplace_back(Counter, &workerCounts [i]);
	}

	// Give ~10 seconds of runtime
	std::this_thread::sleep_for(std::chrono::seconds(10));

	// Stop all threads
	running = false;

	// Wait for all workers to complete
	for (auto &t : workerThreads)
	{
		if (t.joinable()) 
		{
			t.join();
		}
	}

	// Output results
	uint64_t total = 0;
	std::ofstream countFile("counts-v1.txt");
	for (auto i = 0; i < workers; i ++)
	{
		total += workerCounts [i];
		countFile << "Worker " << i + 1 << " processed " << workerCounts [i] << " items" << std::endl;
	}

	countFile << "Total " << total << " items" << std::endl;
	countFile.close();

	// Remove count array
	delete[] workerCounts;
	
	return 0;
}
