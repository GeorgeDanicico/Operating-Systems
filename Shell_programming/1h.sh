#!/bin/bash

if test $# -lt 1; then
	echo Invalid parameters
	exit 1
fi

if ! test -d $1; then
	echo "$1 is not a directory"
	exit 1
fi

total=0


for file in $(ls -all "$1" | awk '{print $5}')
do
	size=$(echo "$file" )
	total=$((total+size))
done

echo "The total size is $total"
