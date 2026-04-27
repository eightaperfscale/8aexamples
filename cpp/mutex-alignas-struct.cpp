// Example source code showing variables aligned in a structure to the CL_SIZE 
// define which can be passed in from a command line option like 
// -DCL_SIZE=xx (Linux) or /DCL_SIZE=xx (Windows)
//
// The source code is included as-is with no warranty.
//
#include <iostream>
#include <thread>
#include <mutex>

typedef struct locks
{
	alignas (CL_SIZE) std::mutex strLock;
	alignas (CL_SIZE) std::mutex intLock;
	alignas (CL_SIZE) int notUsed;
} locks_t;

locks_t ourLocks;

#define LOCKIT(lk)		ourLocks.lk.lock();
#define UNLOCKIT(lk)	ourLocks.lk.unlock();

alignas(CL_SIZE) int i;
std::string s = "";

void DoWork (int value)
{
	//	Restrict access to the int
	LOCKIT(intLock);
	i += value;
	UNLOCKIT(intLock);

	//	Restrict access to the string
	LOCKIT(strLock);
	s += char('A' + value);
	UNLOCKIT(strLock);
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
