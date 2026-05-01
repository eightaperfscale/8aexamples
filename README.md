# 8aexamples
## Welcome!

This repository holds source code in support of the 8 A's of Performance and Scalability book. The source files are slightly different than the examples in the book due to code reuse and portability between Linux and Windows.

The source code is included as-as with no warranty.

## File Structure

The repository has the following subdirectories:

| Subdirectory | Notes |
| ------------ | ----- |
| .            | The root directory contains the build.sh and build.bat build scripts. |
| asm   | This subdirectory contains some AMD/Intel x64 assembly language source code and a test program, in C, to run the assembled machine code. |
| cpp   | Contains a number of example C and C++ source files that will compile on both Linux and Windows. |
| cpp/win   | This subdirectory holds C++ source code that is only relevant for Windows. |
| primes/python   | Contains various Python examples to show different prime numbers algorithms and processing models. The code requires Python3, and was tested with Python 3.12.3 and 3.14.3. |
| primes/cpp   | Contains a C version of the fastest Python prime number algorithm. |

## General Notes

C and C++ source files are built using a build.sh script (Linux) or build.bat batch file (Windows). The scripts do not use any compiler optimization flags, but these can be added manually. However, adding optimizations may exclude code from some of the executables as a number of the examples demonstrate a particular access or alignment issue and don't produce meaningful output.

C and C++ source files will compile on both Linux and Windows, except for the source code in the cpp/win directory which contains Windows affinity examples.

Python examples are cross-platform and should work on both Linux and Windows.

## Building on Linux

The source code examples use gcc, g++ and the hwloc library. To install the components, open a terminal and run the following commands:

        sudo apt install gcc g++ hwloc build-essential

To build the source code, open a terminal and run the following commands:

        cd
        git clone https://github.com/eightaperfscale/8aexamples.git
        cd 8aexamples
        chmod 755 build.sh
        ./build.sh

Executables will be located in the output subdirectory.

## Building on Windows

Download a ZIP File containing the source code from https://github.com/eightaperfscale/8aexamples. Extract the ZIP file and note the location where the source code is extracted. For the purposes of this example it is assumed the location is C:\temp\8aexamples.

The source code examples require the Community Edition of Visual Studio which is free and can be downloaded from https://visualstudio.microsoft.com/.

The examples require a Visual Studio Command Prompt which can be opened by pressing the Windows key or clicking on the Windows launch icon. Change the view to list:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/3a994380-ce5b-454a-a432-a00c8a607d01" />

Locate Visual Studio 2026 (or 2022) and click on the menu list:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/26fb794d-5ec4-426e-a649-4bbf9c65d572" />


Select the Native Tools Command Prompt:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/09e5e448-f3f2-4d6c-9a57-8b9700e788b7" />


Run the following commands to build the source code - alter the paths to match your own configuration:

        cd C:\temp\8aexamples
        build.bat

Executables will be located in the output subdirectory.

## Assembler General Information

The asm directory contains two .s source files that can be assembled with the NASM, and probably the YASM, assemblers. The source code is for AMD and Intel x64 processors. The directory also contains a testharness.c source file that can be compiled with gcc (Linux) or Visual Studio (Windows) and used to run output from NASM. The testharness executable will be built by the build scripts described earlier in this README.

By default, NASM assembles machine code into a flat binary file. The code is position independent and can be loaded and executed like dynamic (JIT) code by the testharness executable. 

Neither of the .s file following any platform ABI as they are mostly alignment examples. However, they will run through the testharness.

## Assembler Examples (Linux)

To install NASM on Linux, open a terminal and run the following command:

        sudo apt install nasm

To assemble the examples on Linux, open a terminal and run the following commands:

        cd
        cd 8aexamples/asm
        nasm spinlock-v1.s
        nasm spinlock-v2.s

Machine code will be located in the spinlock-v1 and spinlock-v2 files. To run the samples:

        ../output/testharness spinlock-v1
        ../output/testharness spinlock-v2

The RAX register is used to return a result from a function. The testharness will output the value of the RAX register which should be 0.

## Assembler Examples (Windows)

The NASM assembler can be downloaded from https://www.nasm.us/.

To assemble the examples on Windows, open a Command Prompt run the following commands (assume path from ZIP file extract):

        cd
        cd C:\temp\8aexamples\asm
        nasm spinlock-v1.s
        nasm spinlock-v2.s

Machine code will be located in the spinlock-v1 and spinlock-v2 files. An elevated Command Prompt might be required e.g., run the Command Prompt as Administrator. To run the samples:

        ..\output\testharness spinlock-v1
        ..\output\testharness spinlock-v2

The RAX register is used to return a result from a function. The testharness will output the value
of the RAX register which should be 0.
