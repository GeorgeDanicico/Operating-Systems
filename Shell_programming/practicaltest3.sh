#!/bin/bash

if test $# -lt 1; then
	echo "Invalid input"
	exit 1
fi

if ! test -d "$1"; then
	echo "$1 is not a directory"
	exit
fi

files=$(find "$1" -type f)
strings="\n"
count2=0
for file in $files
do
	c="C"
	file_type=$(file $file | awk '{print $2}')
	if test $file_type = $c; then
		f=$(echo $file | grep -E ".*\.c" | wc -l)
		if test $f -eq 1; then
			strings+=$file'\n'
		else
			count2=`expr $count + $(grep -E "" $file | wc -l)`
		fi
	fi

done

echo -e "The C files with extension .c are: $strings"
echo "The number of lines in the C files without the .c extension is: $count2"
