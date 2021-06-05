#!/bin/bash

if test $# -lt 1; then
	echo "Invalid arguments."
	exit 1
fi

if ! test -d "$1"; then
	echo "$1 is not a directory"
	exit 1
fi

files=$(find "$1" -type f)

for file in $files
do
	ext=$(echo $file | grep -E ".txt" | wc -l)
	if test $ext -eq 0; then
		echo "File without .txt: $file"
	else 
		content=$(sed -E "y/aeiou/AEIOU/" "$file")
		echo The content of the file $file is:
		echo $content
	fi

done 
