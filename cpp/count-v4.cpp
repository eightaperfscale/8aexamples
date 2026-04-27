// Example source code that shows affinity with physical cores filtered and 
// cache line alignment to avoid false sharing
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include <cmath>

#ifdef _WIN32
#include "win\filtercores.h"
#else  // _WIN32
#include <hwloc.h>
#include <unistd.h>
#include "filtercores.h"
#endif // _WIN32

// Worker processing counts
struct alignas(64) countStruct
{
	uint64_t count;
};

countStruct *workerCounts;

std::atomic<bool> running = true;

// Count worker
void Counter (uint64_t *count)
{
	while (running)
		(*count) ++;
}

int main (void)
{
	//	Total cores on the platform
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto totalCores = sysinfo.dwNumberOfProcessors;
#else  // _WIN32
	auto totalCores = sysconf(_SC_NPROCESSORS_ONLN);
#endif // _WIN32

	//	Locate physical cores
    std::vector<int> cores;
	FilterCores(cores);
	auto workers = cores.size();

	// Allocate a buffer to track how much each worker processes
	workerCounts = new countStruct [workers]();

	// Create workers
	std::vector<std::thread> workerThreads;
	for (auto i = 0; i < workers; i ++)
	{
		//	Create and track the worker thread
		workerThreads.emplace_back(Counter, &workerCounts [i].count);

		// Apply affinity to the thread
#ifdef _WIN32
		auto rc = SetThreadAffinityMask(workerThreads [i].native_handle(), 1ULL << cores.at(i));
		if (rc == 0)
		{
			auto err = GetLastError();
			std::cerr << "Affinity failed for thread " << i << " return code " << err << std::endl;
		}
#else  // _WIN32
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(cores [i], &cpuset);
		auto rc = pthread_setaffinity_np(workerThreads [i].native_handle(), sizeof(cpu_set_t), &cpuset);
		if (rc != 0)
		{
			std::cerr << "Affinity failed for thread " << i << " return code " << rc << std::endl;
		}
#endif // _WIN32
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
	std::ofstream countFile("counts-v4.txt");
	for (auto i = 0; i < workers; i ++)
	{
		total += workerCounts [i].count;
		countFile << "Worker " << i + 1 << " processed " << workerCounts [i].count << " items" << std::endl;
	}

	countFile << "Total " << total << " items" << std::endl;
	countFile.close();

	// Remove count array
	delete[] workerCounts;
	
	return 0;
}
