#!/bin/bash

if test $# -lt 1; then
	echo "Invalid arguments."
	exit 1
fi

if test -z "$1"; then
	echo "Invalid extension"
	exit 1
fi

files=$(find . -type f -name "*$1")

if test -z "$files"; then
	echo "there is no file with that extension"
	exit 1
fi


for file in $files
do
	count=$(grep -E "\<[0-9]*[02468]\>" $file | wc -l)
	if test $count -eq 0; then
		count=$(grep -E "" $file | wc -w)
		if test $((count % 2)) -eq 1; then
			perm=$(ls -all $file | awk '{print $1, $9}')
			echo "$perm"

		fi
	fi
done
	
