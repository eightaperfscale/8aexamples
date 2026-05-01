// This source code demonstrates how to obtain the cache line size.
//
// The source code is included as-is with no warranty.
//
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

// Pointer conversion
#define TO_SLPI_PTR(p) (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX) p

int GetCLSize (void)
{
    // Find size of the buffer
    DWORD procInfoSize = 0;
    if (!GetLogicalProcessorInformationEx(RelationAll, nullptr, &procInfoSize)) 
    {
        auto err = GetLastError();
        if (err != ERROR_INSUFFICIENT_BUFFER) 
        {
            std::string msg = "Unable to locate processor information. Error " + std::to_string(err);
            throw std::runtime_error(msg);
        }
    }

    // Allocate the processor buffer, now we have the size
    std::vector<uint8_t> procInfoBuf (procInfoSize);

    // Get processor information
    if (!GetLogicalProcessorInformationEx(RelationAll, TO_SLPI_PTR(procInfoBuf.data()), &procInfoSize))
    {
        auto err = GetLastError();
        std::string msg = "Unable to access processor information. Error " + std::to_string(err);
        throw std::runtime_error(msg);
    }

    // Iterate over the buffer find cache information
    DWORD entryOffset = 0;
    while (entryOffset < procInfoSize)
    {
        // Get a pointer to the current entry
        auto platInfo = TO_SLPI_PTR((procInfoBuf.data() + entryOffset));
        if (platInfo->Relationship == RelationCache)
        {
			return platInfo->Cache.LineSize;
        }

        // Next entry
        entryOffset += platInfo->Size;
    }

	// No valid cache line found
	return 0;
}    

int main (void)
{
    try
    {
        // Find the cache line size
        auto CLSize = GetCLSize();

		// The cache line must be non-zero and a power of 2
		if (CLSize == 0 || (CLSize & (CLSize - 1)) != 0)
		{
			std::cerr << "Invalid cache line size" << std::endl;
			return 1;
		}
		
	    std::cout << "The cache line size is " << CLSize << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
