// Example source code to filter physical cores.
//
// The source code is included as-is with no warranty.
//

// E-core and P-core values (may vary on other platforms)
constexpr BYTE ECore = 0;
constexpr BYTE PCore = 1;

// Pointer conversion
#define TO_SLPI_PTR(p) (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX) p

// Filter out the best platform cores
void FilterCores (std::vector<int> &cores)
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

    // Iterate over the buffer to count the number of cores per Processor Group
    DWORD entryOffset = 0;
    std::vector<int> groupCount;
    while (entryOffset < procInfoSize)
    {
        // Get a pointer to the current entry
        auto platInfo = TO_SLPI_PTR((procInfoBuf.data() + entryOffset));
        if (platInfo->Relationship == RelationGroup)
        {
            // Sum cores in all Processor Groups
            for (auto i = 0; i < platInfo->Group.ActiveGroupCount; i ++) 
            {
                // Logical cores in this Processor Group
                auto coresThisGroup = (int) platInfo->Group.GroupInfo [i].ActiveProcessorCount;
                if (i != 0)
                {
                    // Produce running total of cores
                    coresThisGroup += groupCount[i - 1];
                }

                // Track
                groupCount.push_back(coresThisGroup);
            }
        }

        // Next entry
        entryOffset += platInfo->Size;
    }

    // Iterate over the buffer to find the best cores
    entryOffset = 0;
    while (entryOffset < procInfoSize) 
    {
        // Get a pointer to the current entry
        auto platInfo = TO_SLPI_PTR((procInfoBuf.data() + entryOffset));

        if (platInfo->Relationship == RelationProcessorCore) 
        {
            // Core information - only want P-cores
            if (platInfo->Processor.EfficiencyClass == PCore)
            {
                // Find the lowest bit set the Processor Group
                DWORD index;
                if (BitScanForward64(&index, platInfo->Processor.GroupMask [0].Mask) == 0)
                {
                    throw std::runtime_error("Could not locate logical core");
                }

                // The core number will be the index within a Processor Group and
                // needs to adjusted to give a platform logical core number
                auto ourGroup = platInfo->Processor.GroupMask [0].Group;
                int coreNum = (ourGroup == 0)
                  ? index 
                  : index + groupCount [ourGroup];
                    
                // Track the core
                cores.push_back(coreNum);
            }
        }

        // Next entry
        entryOffset += platInfo->Size;
    }
}
