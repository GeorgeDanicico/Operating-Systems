#!/bin/bash

if test $# -lt 1; then
	echo "Invalid arguments"
	exit 1
fi

if ! test -d $1; then
	echo "$1 is not a directory."
	exit 1
fi

count=0

files=$(find "$1" -type f -name "*.c")

for file in $files
do
	lines=$(awk '{print NR} ' $file | wc -w)
	if test $lines -gt 500; then
		count=`expr $count + 1`
		echo $file
		if test $count -eq 2; then
			exit 1
		fi
	fi

done
