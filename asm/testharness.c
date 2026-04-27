// Example source code that implements a test harness for NASM assembled 
// machine code. Compile the C code and run with:
//
//		./testharness <bin file>
//
// By default NASM assembles a flat binary file with position 
// independant code. Use this code to run with this sample
// to validate the NASM code works. For example:
//
//		nasm spinlock-v1.s
//		./testharness spinlock-v1
//
// The source code is included as-is with no warranty.
//

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif // _WIN32

// Help portability
#ifdef _WIN32
#define FREE_CODE(p) VirtualFree(p, 0, MEM_RELEASE);
#else  // _WIN32
#define FREE_CODE(p) free(p)
#endif // _WIN32

// Function pointer typedef
typedef int (*nasmCode) (void);

// Union to set the function pointer address so
// the NASM code can be called
union funcPtrInfo
{
	nasmCode theCode;
	char* theAddr;
};

// Get the platform page size
int GetPageSize (void)
{
#ifdef _WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return sysInfo.dwPageSize;
#else  // _WIN32
    return sysconf(_SC_PAGE_SIZE);
#endif // _WIN32
}

// Get memory for the NASM code
char *GetCodeMemory (const size_t codeSize, const size_t pgSize, int *err)
{
#ifdef _WIN32
	// Windows version
	char *codeMem = (char *) VirtualAlloc(NULL, codeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	*err = GetLastError();
#else  // _WIN32
	// Linux version
	char *codeMem = (char *) aligned_alloc(pgSize, codeSize);
	*err = errno;
#endif // _WIN32

	return codeMem;
}

// Read an entire file
char *ReadEntireFile (FILE *inFile, size_t *codeSize)
{
	// Get the file size
    fseek(inFile, 0, SEEK_END);
	size_t fileSize = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);

	// Round the file size to the next page size
    int pgSize = GetPageSize();
    int pgMask = pgSize - 1;
    *codeSize = (fileSize + pgMask) & ~pgMask;

	//	Get the memory for the NASM code
	int err;
	char *codeMem = GetCodeMemory(*codeSize, pgSize, &err);
	if (codeMem == NULL)
	{
		*codeSize = 0;
		fprintf(stderr, "Unable to allocate memory for %ld bytes, error %d\n", *codeSize, errno);
		return NULL;
	}

#ifndef _WIN32
	// Set memory attributes so the code will execute
	int rc = mprotect(codeMem, *codeSize, PROT_READ | PROT_EXEC | PROT_WRITE);
	if (rc == -1)
	{
		*codeSize = 0;
		fprintf(stderr, "Unable to set memory attributes, error %d\n", errno);
		return NULL;
	}
#endif  // _WIN32

	//	Read the code
	size_t bytesRead = fread(codeMem, 1, fileSize, inFile);
	if (bytesRead != fileSize)
	{
		*codeSize = 0;
		fprintf(stderr, "Expected to read %ld bytes, but got %ld\n", fileSize, bytesRead);
		return NULL;
	}

	return codeMem;
}

int main (int argc, char **argv)
{
	// Validate input
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <NASM binary file>\n", argv [0]);
		return 1;
	}

	// Open the file
	FILE *inFile = fopen(argv [1], "rb");
	if (inFile == NULL)
	{
		fprintf(stderr, "Unable to open %s, error %d\n", argv [1], errno);
		return 1;
	}

	// Read the entire file
	size_t codeSize;
	char *code = ReadEntireFile(inFile, &codeSize);
    if (codeSize != 0)
	{
		// Set the address of the NASM code
		union funcPtrInfo ourCode;
		ourCode.theAddr = code;

		// Call into the NASM code
		int rc = ourCode.theCode();

		printf("The result was %d\n", rc);
	}

	// Clean up
	FREE_CODE(code);
	fclose(inFile);

	return 0;
}
