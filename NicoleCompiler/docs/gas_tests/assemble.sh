#!/bin/bash

LIBRARIES="-L./libs/ -lNicoleFramework"

if [[ -z "$1" ]]
then
	printf "Specify the file.\n"
	exit 1
fi
if [[ -n "$2" ]]
then
	LIBRARIES="$2"
fi

g++ -m32 -o test "$1" $LIBRARIES || exit $?

./test
