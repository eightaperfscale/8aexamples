// This source code demonstrates how to locate and filter logical cores.
//
// The source code is included as-is with no warranty.
//
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "filtercores.h"

int main(void)
{
    std::vector<int> cores;
    try
    {
        // Find the best cores
        FilterCores(cores);

        // Output the cores found
        for (auto& c : cores)
        {
            std::cout << "Logical core " << c << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}




