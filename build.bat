@echo off

rem Create output directory if it does not exist
if not exist "output" (mkdir output)

rem Build C code
for /R . %%i in (*.c) do (
	echo | set /p="Building "
	cl /nologo /W0 /Zi /Feoutput\%%~ni.exe %%i
	del %%~ni.obj
)

rem Build C++ code
for /R . %%i in (*.cpp) do (
	echo | set /p="Building "
	cl /nologo /W0 /Zi -DCL_SIZE=64 /std:c++17 /EHsc /I primes/cpp /Feoutput\%%~ni.exe %%i
	del %%~ni.obj
)
