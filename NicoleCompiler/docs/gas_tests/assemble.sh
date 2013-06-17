#!/bin/bash

LIBRARY_PATH="./libs/"

if [ -z "$1" ]
then
	printf "Specify the file.\n"
	exit 1
fi
if [ ! -z "$2" ]
then
	LIBRARY_PATH="$2"
fi

g++ -m32 -o test "$1" -L"$LIBRARY_PATH" -lNicoleFramework || exit $?

./test
