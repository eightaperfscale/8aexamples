// Example source code showing variables aligned to the CL_SIZE define which 
// can be passed in from a command line option like -DCL_SIZE=xx (Linux) or
// /DCL_SIZE=xx (Windows)
//
// The source code is included as-is with no warranty.
//
#include <iostream>
#include <thread>
#include <mutex>

alignas(CL_SIZE) std::mutex strLock;
alignas(CL_SIZE) std::mutex intLock;

int i = 0;
std::string s = "";

void DoWork (int value)
{
	//	Restrict access to the int
	intLock.lock();
	i += value;
	intLock.unlock();

	//	Restrict access to the string
	strLock.lock();
	s += char('A' + value);
	strLock.unlock();
}

int main (void)
{
	//	Start threads
	std::thread t1(DoWork, 1);
	std::thread t2(DoWork, 2);

	t1.join();
	t2.join();

	std::cout << "String is " << s << " int is " << i << std::endl;

	return 0;
}
