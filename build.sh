# !/bin/bash
#
# Build all code examples

# Create output directory
if [ ! -d output ]
then
	mkdir output/ 2>/dev/null
fi

# Build C code
for i in `find . -name "*.c" -print`
do
	echo "Building "$i
	e=`basename $i .c`
	gcc -g $i -o output/$e
done

# Build C++ code
cl=`getconf LEVEL1_DCACHE_LINESIZE`
for i in `find . -name "*.cpp" -print | grep -v "/"win"/"`
do
	echo "Building "$i
	e=`basename $i .cpp`
	g++ -g -std=c++17 -DCL_SIZE=$cl $i -o output/$e -lm -lhwloc -march=native
done

echo "Done"

