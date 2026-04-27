// Source code example with coarse mutex.
//
// The source code is included as-is with no warranty.
//
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

std::map<int, std::string> mappedData;

std::mutex mapLock;

void MapWorker (void)
{
	std::lock_guard<std::mutex> lock(mapLock);

#ifdef _WIN32
	// Seed random based on time and thread id
    std::srand(unsigned(std::time(0) ^ GetCurrentThreadId()));
#endif // _WIN32

	// Generate a random key and string
	auto key = rand() % 4096;
	mappedData [key] = std::tmpnam(nullptr);
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
		workerThreads.emplace_back(MapWorker);
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
	for (auto const& [key, value] : mappedData)
	{
		std::cout << "The key is " << key << " with value " << value << std::endl;
	}

	return 0;
}



