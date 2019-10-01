#!/bin/sh

BASE_DIR=`pwd`
TOOLS_DIR=$BASE_DIR/tools
tools=`ls $TOOLS_DIR`

#rm -rf Makefile
first_line="all: m.o"
for var in $tools; do
    first_line="$first_line $var.o"
done
echo $first_line
echo "	gcc *.o -o m"
echo "m.o:"
echo "	gcc -c m.c"

for var in $tools; do
	echo "$var.o:"
	echo "	gcc -c tools/$var/$var.c"
done

echo "clean:"
echo "	rm -rf *.o m"
