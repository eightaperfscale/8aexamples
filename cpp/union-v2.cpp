// Example source code showing a corrected store forwarding issue.
//
// The source code is included as-is with no warranty.
//
#include <iostream>

union someUnion 
{
	int x;
	char bytes [4];
};

int main (void)
{
	someUnion ourUnion;

	ourUnion.x = 0x41424344;

	std::cout << "X is " << std::hex << ourUnion.x << std::endl;
	std::cout << "Byte 1 is " << ourUnion.bytes [0] << std::endl;

	return 0;
}

