#!/bin/bash

if test $# -lt 1; then
	echo "Invalid arguments."
	exit 1
fi

# if directory

files=$(find "$1" -name "*.txt")
for file in $files
do
	cat $file | sort >> new_file
	cat new_file >> $file
	rm new_file


done

