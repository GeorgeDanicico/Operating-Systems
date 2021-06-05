#!/bin/bash

# check if it is a directory

files=$(find "$1")

for file in $files:
do
	if test -L $file && test -e $file; then
		echo "$file"
	fi
done


		
